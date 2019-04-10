/*
 * Copyright (C) 2017 ~ 2018 Wuhan Deepin Technology Co., Ltd.
 *
 * Author:     Iceyer <me@iceyer.net>
 *
 * Maintainer: Iceyer <me@iceyer.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "bootmaker.h"

#include <QDebug>
#include <XSys>

#include "../util/sevenzip.h"
#include "../util/utils.h"
#include "../util/devicemonitor.h"

#include "diskutil.h"

#ifdef Q_OS_WIN32
#include <windows.h>
#include <shellapi.h>
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "advapi32.lib")
#endif

#ifdef Q_OS_LINUX
#include <unistd.h>
#include <sys/reboot.h>
#endif

class Error
{
public:
    enum ErrorType {
        NoError = 0,
        SyscExecFailed,
        USBFormatError,
        USBSizeError,
        USBMountFailed,
        ExtractImgeFailed,
    };
};

const QString BMHandler::errorString(BMHandler::ErrorType et)
{
    switch (et) {
    case NoError:
        return "";
    case SyscExecFailed:
        return BMHandler::tr("Failed to call the command %1");
    case USBFormatError:
        return BMHandler::tr("Disk Format Error: Please format the disk with FAT32");
    case USBSizeError:
        return BMHandler::tr("Insufficient Disk Space: Ensure the disk has %1 free space");
    case USBMountFailed:
        return BMHandler::tr("Disk Mount Error: Insert the disk again or reboot to retry");
    case ExtractImgeFailed:
        return BMHandler::tr("Image Decompression Error: Verify md5 checksum of the image to ensure its integrity");
    }
    return BMHandler::tr("Internal Error");
}


BootMaker::BootMaker(QObject *parent) : BMHandler(parent)
{
    m_usbDeviceMonitor = new DeviceMonitor;
    QThread *monitorWork = new QThread;
    m_usbDeviceMonitor->moveToThread(monitorWork);
//    connect(monitorWork, &QThread::started,
//            m_usbDeviceMonitor, &DeviceMonitor::startMonitor);
    connect(m_usbDeviceMonitor, &DeviceMonitor::removablePartitionsChanged,
            this, &BootMaker::removablePartitionsChanged);

    monitorWork->start();

    connect(this, &BootMaker::finished, this, [ = ](int errcode, const QString & description) {
        this->reportProgress(100, errcode, "install failed", description);
    });
}

void BootMaker::reboot()
{
#ifdef Q_OS_WIN32
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0);
//    ExitWindowsEx(EWX_REBOOT, EWX_FORCE);
#endif
#ifdef Q_OS_LINUX
    sync();
    ::reboot(RB_AUTOBOOT);
#endif
#ifdef Q_OS_MAC
    XSys::SynExec("shutdown", "-r now &");
#endif
}

void BootMaker::start()
{
    emit m_usbDeviceMonitor->startMonitor();
}

void BootMaker::stop()
{
    emit m_usbDeviceMonitor->pauseMonitor();
}

const QList<DeviceInfo> BootMaker::deviceList() const
{
    return m_usbDeviceMonitor->deviceList();
}

bool BootMaker::install(const QString &image, const QString &unused_device, const QString &partition, bool formatDevice)
{
    emit m_usbDeviceMonitor->pauseMonitor();

    qDebug() << image << unused_device << partition << formatDevice;

    //check iso integrity
    SevenZip sevenZipCheck(image, "");
    if (!sevenZipCheck.check()) {
        qCritical() << "Error::get(Error::ExtractImgeFailed)";
        emit finished(ExtractImgeFailed, errorString(ExtractImgeFailed));
        return false;
    }
#ifdef Q_OS_UNIX
    QString device = XSys::DiskUtil::GetPartitionDisk(partition);
#else
    QString device = partition;
#endif

    this->reportProgress(5, Error::NoError, "install bootloader", "");
    qDebug() << "begin install bootloader on" << partition;
    QString targetPartition = partition;
    XSys::Result result;

#ifdef DBM_NO_BOOTLOADER
    qWarning() << "make disk with no bootloader mode";
    if (formatDevice) {
        XSys::SynExec("umount", targetPartition);

        QStringList args;
        args << "-n" << "DEEPINOS" << targetPartition;
        XSys::SynExec("mkfs.fat", args.join(" "));

        XSys::DiskUtil::Mount(targetPartition);
    }
    qDebug() << "format disk: " << result.isSuccess();
#else
    if (formatDevice) {
        result = XSys::Bootloader::InstallBootloader(device);
        targetPartition = result.result();
    } else {
        result = XSys::Bootloader::Syslinux::InstallSyslinux(partition);
    }
    qDebug() << "install bootloader finish: " << result.isSuccess();
#endif
    if (! result.isSuccess()) {
        qCritical() << "install bootloader failed: " << result.errmsg();
        emit finished(SyscExecFailed, errorString(SyscExecFailed).arg(result.cmd()) + " " + result.errmsg());
        return false;
    }

    this->reportProgress(10, Error::NoError, "begin reload disk", "");
    qDebug() << "begin reload disk";

    QString installDir = partition;
#ifdef Q_OS_UNIX
    installDir = XSys::DiskUtil::MountPoint(partition);
    if (installDir.isEmpty()) {
        qCritical() << "Error::get(Error::USBMountFailed)";
        emit finished(USBMountFailed, errorString(USBMountFailed));
        return false;
    }
#endif

    this->reportProgress(15, Error::NoError, "clear target device files", "");
    qDebug() << "begin clear target device files";
    Utils::ClearTargetDev(installDir);

    this->reportProgress(20, Error::NoError, "extract files", "");
    this->reportProgress(20, Error::NoError, "begin extract files", "");
    SevenZip sevenZip(image, installDir);
    connect(sevenZip.m_szpp, &SevenZipProcessParser::progressChanged,
    m_usbDeviceMonitor, [ = ](int current, int /*total*/, const QString & fileName) {
//        qDebug() << current << total << fileName;
        emit this->reportProgress(current * 60 / 100 + 20, Error::NoError, "extract", fileName);
    }, Qt::QueuedConnection);

    if (!sevenZip.extract()) {
        qCritical() << "Error::get(Error::ExtractImgeFailed)";
        emit finished(ExtractImgeFailed, errorString(ExtractImgeFailed));
        return false;
    }
    this->reportProgress(80, Error::NoError, "end extract files", "");
    this->reportProgress(80, Error::NoError, "config syslinux", "");
    XSys::Bootloader::Syslinux::ConfigSyslinx(installDir);

#ifdef Q_OS_UNIX
    this->reportProgress(81, Error::NoError, "begin syncing filesystems", "");
    XSys::SynExec("sync", "");
    this->reportProgress(94, Error::NoError, "begin syncing filesystems", "");
#endif

    this->reportProgress(95, Error::NoError, "eject disk", "");
    XSys::DiskUtil::EjectDisk(partition);

    this->reportProgress(100, Error::NoError, "finish", "");
    return true;
}
