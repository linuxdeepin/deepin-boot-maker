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

#include "../util/sevenzip.h"
#include "../util/utils.h"
#include "../util/devicemonitor.h"
#include "diskutil.h"
#include <QDebug>
#include <QFileInfo>

#include <XSys>



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
        return BMHandler::tr("Failed to call the command 1%");
    case USBFormatError:
        return BMHandler::tr("Disk Format Error: Please format the disk with FAT32");
    case USBSizeError:
        return BMHandler::tr("Insufficient Disk Space: Ensure the disk has 1% free space");
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
        this->reportProgress(101, errcode, "install failed", description);
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

    qDebug() << "BootMaker start";
    emit m_usbDeviceMonitor->startMonitor();
}

void BootMaker::stop()
{
    emit m_usbDeviceMonitor->pauseMonitor();
}

const QList<DeviceInfo> BootMaker::deviceList() const
{
    qDebug() << "BootMaker deviceList";
    return m_usbDeviceMonitor->deviceList();
}
bool BootMaker::checkfile(const QString &filepath)
{
    qDebug() << "CheckFile:" << filepath;
    //check iso integrity
    SevenZip sevenZipCheck(filepath, "");
    if (!sevenZipCheck.check()) {
        qCritical() << "Error::file check error";
//        emit checkFileResult(false);
        return false;
    }
//    emit checkFileResult(true);
    return true;
}

bool BootMaker::install(const QString &image, const QString &unused_device, const QString &partition, bool formatDevice)
{
    emit m_usbDeviceMonitor->pauseMonitor();

    qDebug() << image << unused_device << partition << formatDevice;
    QFileInfo isoInfo(image);

#define KByt 1024
    if (formatDevice) {
        if (isoInfo.size() / KByt > XSys::DiskUtil::GetPartitionTotalSpace(partition)) {
            qCritical() << "Error::get(Error::USBSizeError)";
            emit finished(USBSizeError, errorString(USBSizeError));
            return false;
        }
    } else {
        if (isoInfo.size() / KByt > XSys::DiskUtil::GetPartitionFreeSpace(partition)) {
            qCritical() << "Error::get(Error::USBSizeError)";
            emit finished(USBSizeError, errorString(USBSizeError));
            return false;
        }
    }


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
        result = XSys::SynExec("umount", targetPartition);
        qDebug() << "umount disk: " << result.isSuccess();
        auto targetDisk = XSys::DiskUtil::GetPartitionDisk(targetPartition);
        QStringList args;
        args << targetDisk << "-I" ;
        XSys::SynExec("umount", targetDisk);
        XSys::SynExec("mkfs.fat", args.join(" "));
        result = XSys::SynExec("parted", QString(" -s -a optimal %1 mklabel msdos").arg(targetDisk));
        qDebug() << "parted  -s -a optimal %1 mklabel msdos " << targetDisk;
        qDebug() << "format mklabel: " << result.isSuccess();
        //fix bug 18668 arm和龙芯制作格式化使u盘变小，现在格式化制作如实大小
        result = XSys::SynExec("parted", QString("-s -a optimal %1 mkpart primary fat32 0% 100%").arg(targetDisk));
        qDebug() << "format mkpart: " << result.isSuccess();
        targetPartition = targetDisk + "1";
        XSys::SynExec("partprobe", "");
        XSys::SynExec("partprobe", "");
        QStringList args1;
        XSys::Result ret7 = XSys::SynExec("isoinfo", QString("-i %1 -d").arg(image));
        if (!ret7.isSuccess()) {
            qWarning() << "call isoinfo failed" << ret7.result();
        }
        QStringList volume = ret7.result().split("\n").filter("Volume id");
        QString tmp = volume.takeAt(0);
        if (tmp.contains("deepin", Qt::CaseInsensitive)) {
            args1 << "-n" << "DEEPINOS" << targetPartition ;
        } else if (tmp.contains("uos", Qt::CaseInsensitive)) {
            args1 << "-n" << "UOS" << targetPartition ;
        } else {
            args1 << "-n" << "UNKNOWN" << targetPartition ;
        }
        XSys::SynExec("umount", targetPartition);
        XSys::SynExec("umount", targetPartition);
        XSys::SynExec("umount", targetPartition);
        XSys::SynExec("mkfs.fat", args1.join(" "));
        qDebug() << "format partation: " << targetPartition << result.isSuccess();
//        XSys::DiskUtil::Mount(targetPartition);
    }
    XSys::DiskUtil::Mount(targetPartition);
#else
    if (formatDevice) {
        result = XSys::Bootloader::InstallBootloader(device, image);
        targetPartition = result.result();
    } else {
        result = XSys::Bootloader::Syslinux::InstallSyslinux(partition, image);
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
//fix bug 18693 12288 如果u盘有多个分区挂载失败的问题
#ifdef Q_OS_UNIX
#ifdef Q_OS_LINUX
    device = device + "1";
#endif
    installDir = XSys::DiskUtil::MountPoint(device);
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
        //fix bug 32703,fix Unplug the USB flash disk
        QFileInfo devFile(partition);
        if (!devFile.exists()) {
            qCritical() << "Error::get(Error::USBMountFailed)";
            emit finished(USBMountFailed, errorString(USBMountFailed));
            return false;
        }
        qCritical() << "Error::get(Error::ExtractImgeFailed)";
        emit finished(ExtractImgeFailed, errorString(ExtractImgeFailed));
//        qCritical() << "Error::get(Error::USBSizeError)";
//        emit finished(USBSizeError, errorString(USBSizeError));
        return false;
    }
    this->reportProgress(80, Error::NoError, "end extract files", "");
    this->reportProgress(80, Error::NoError, "config syslinux", "");
    XSys::Bootloader::Syslinux::ConfigSyslinx(installDir);
//#ifdef Q_OS_UNIX
//    this->reportProgress(81, Error::NoError, "begin syncing filesystems", "");
//    XSys::SynExec("sync", "");
//    this->reportProgress(94, Error::NoError, "begin syncing filesystems", "");
//#endif
    this->reportProgress(95, Error::NoError, "eject disk", "");
#ifdef Q_OS_MAC
    XSys::DiskUtil::EjectDisk(partition);
#endif
    this->reportProgress(100, Error::NoError, "finish", "");
//fix bug 12284 更改交互做断电处理，使得同步等待时间合理化
#ifdef Q_OS_LINUX
    XSys::SynExec("sync", "");
    result = XSys::DiskUtil::EjectDisk(partition);
    if (! result.isSuccess()) {
        emit finished(SyscExecFailed, errorString(SyscExecFailed).arg(result.cmd()) + " " + result.errmsg());
    } else {
        this->reportProgress(101, Error::NoError, "finish", "");
    }
#endif
    return true;
}
