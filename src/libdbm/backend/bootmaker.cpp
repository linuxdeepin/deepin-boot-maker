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
#include "qtlinuxinstaller.h"

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
        USBNotMountFailed,
        CheckImageIntegrityFailed,
        ExtractImgeFailed,
        InstallBootloaderFailed,
        GetUsbInstallDirFailed,
        SyncIOFailed,
        UnDefinedError
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
    QtBaseInstaller* pInstaller = nullptr;
    pInstaller->deleteLater();

#ifdef Q_OS_WIN
#else
#ifdef Q_OS_UNIX
    pInstaller = new QtLinuxInstaller;
#else
#endif
#endif

    pInstaller->setImage(image);
    pInstaller->setPartionName(partition);
    pInstaller->setformat(formatDevice);

    connect(pInstaller, &QtBaseInstaller::progressfinished, this, [=](ProgressStatus status, ProgressErorr error) {
        Q_UNUSED(status);
        emit finished(error, errorString(BMHandler::ErrorType(error)));
    });

    connect(pInstaller, &QtBaseInstaller::reportProgress, this, [=](int current, const QString &title, const QString &description){
        emit this->reportProgress(current, Error::NoError, title, description);
    });

    connect(pInstaller->m_sevenZipCheck.m_szpp, &SevenZipProcessParser::progressChanged,
    m_usbDeviceMonitor, [ = ](int current, int /*total*/, const QString & fileName) {
        emit this->reportProgress(current * 60 / 100 + 20, Error::NoError, "extract", fileName);
    }, Qt::QueuedConnection);

    pInstaller->beginInstall();
    emit m_usbDeviceMonitor->startMonitor();
    return true;
}
