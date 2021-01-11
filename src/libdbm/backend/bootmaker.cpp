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
#include "../installer/qtinstallerfactory.h"

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

BootMaker::BootMaker(QObject *parent) : BMHandler(parent)
  ,m_pInstaller(nullptr)
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
    qInfo() << "BootMaker start";

    if (m_pInstaller == nullptr) {
        m_pInstaller = QtInstallerFactory::getInstance()->createInstaller();

        if (m_pInstaller != nullptr) {
            connect(m_pInstaller, &QtBaseInstaller::progressfinished, this, [=](ProgressStatus status, BMHandler::ErrorType error) {
                Q_UNUSED(status);
                emit finished(error, errorString(BMHandler::ErrorType(error)));
            });

            connect(m_pInstaller, &QtBaseInstaller::reportProgress, m_usbDeviceMonitor, [=](int current, const QString &title, const QString &description){
                emit this->reportProgress(current, ErrorType::NoError, title, description);
            });

            connect(m_pInstaller->m_sevenZipCheck.m_szpp, &SevenZipProcessParser::progressChanged,
            m_usbDeviceMonitor, [ = ](int current, int /*total*/, const QString & fileName) {
                emit this->reportProgress(current * 60 / 100 + 20, ErrorType::NoError, "extract", fileName);
            }, Qt::QueuedConnection);
        }
    }

    if (m_pInstaller != nullptr) {
        if (m_pInstaller->isRunning()) {
            qInfo() << "Installer is running";
            m_pInstaller->stopInstall();
        }
        else {
            qInfo() << "Start flush disk";
            emit m_usbDeviceMonitor->startMonitor();
        }
    }
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
    qDebug() << "Begin install" << image << unused_device << partition << formatDevice;

    if (m_pInstaller != nullptr) {
        emit m_usbDeviceMonitor->pauseMonitor();
        m_pInstaller->setImage(image);
        m_pInstaller->setPartionName(partition);
        m_pInstaller->setformat(formatDevice);
        m_pInstaller->beginInstall();
        emit m_usbDeviceMonitor->startMonitor();
    }

    return true;
}
