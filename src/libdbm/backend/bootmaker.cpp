// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "bootmaker.h"
#include "../installer/qtinstallerfactory.h"

#include "../util/sevenzip.h"
#include "../util/utils.h"
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
    qDebug() << "Initializing BootMaker";
    m_usbDeviceMonitor = new DeviceMonitor();

    m_monitorWork = new QThread();
    m_usbDeviceMonitor->moveToThread(m_monitorWork);
    qDebug() << "Device monitor moved to worker thread";

    connect(m_monitorWork, &QThread::finished, m_usbDeviceMonitor, &DeviceMonitor::deleteLater);
    connect(m_usbDeviceMonitor, &DeviceMonitor::removablePartitionsChanged,
            this, &BootMaker::removablePartitionsChanged);

    m_monitorWork->start();
    qDebug() << "Device monitor thread started";

    connect(this, &BootMaker::finished, this, [ = ](int errcode, const QString & description) {
        qInfo() << "Installation finished - Error code:" << errcode << "Description:" << description;
        this->reportProgress(101, errcode, "install failed", description);
    });
}

void BootMaker::reboot()
{
#ifdef Q_OS_WIN32
    qInfo() << "Windows reboot sequence started";
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
    qInfo() << "Linux reboot sequence started";
    sync();
    ::reboot(RB_AUTOBOOT);
#endif
#ifdef Q_OS_MAC
    qInfo() << "macOS reboot sequence started";
    XSys::SynExec("shutdown", "-r now &");
#endif
}

void BootMaker::start()
{
    qInfo() << "Starting BootMaker service";

    if (m_pInstaller == nullptr) {
        qDebug() << "Creating new installer instance";
        m_pInstaller = QtInstallerFactory::getInstance()->createInstaller();

        if (m_pInstaller != nullptr) {
            qDebug() << "Setting up installer connections";
            connect(m_pInstaller, &QtBaseInstaller::progressfinished, this, [=](ProgressStatus status, BMHandler::ErrorType error) {
                Q_UNUSED(status);
                qInfo() << "Installation progress finished - Error type:" << error;
                emit finished(error, errorString(BMHandler::ErrorType(error)));
            });

            connect(m_pInstaller, &QtBaseInstaller::reportProgress, m_usbDeviceMonitor, [=](int current, const QString &title, const QString &description){
                qDebug() << "Progress update - Current:" << current << "Title:" << title;
                emit this->reportProgress(current, ErrorType::NoError, title, description);
            });

            connect(m_pInstaller->m_sevenZipCheck.m_szpp, &SevenZipProcessParser::progressChanged,
            m_usbDeviceMonitor, [ = ](int current, int /*total*/, const QString & fileName) {
                qDebug() << "SevenZip progress - Current:" << current << "File:" << fileName;
                emit this->reportProgress(current * 60 / 100 + 20, ErrorType::NoError, "extract", fileName);
            }, Qt::QueuedConnection);
        } else {
            qWarning() << "Failed to create installer instance";
        }
    }

    if (m_pInstaller != nullptr) {
        if (m_pInstaller->isRunning()) {
            qInfo() << "Installer is already running, stopping current installation";
            m_pInstaller->stopInstall();
        }
        else {
            qInfo() << "Starting disk flush and device monitoring";
            emit m_usbDeviceMonitor->startMonitor();
        }
    }
}

void BootMaker::stop()
{
    qInfo() << "Stopping BootMaker service";
    emit m_usbDeviceMonitor->pauseMonitor();
}

const QList<DeviceInfo> BootMaker::deviceList() const
{
    qDebug() << "Retrieving device list";
    return m_usbDeviceMonitor->deviceList();
}

bool BootMaker::checkfile(const QString &filepath)
{
    qDebug() << "Checking file integrity:" << filepath;
    // //check iso integrity
    // SevenZip sevenZipCheck(filepath, "");

    // if (!sevenZipCheck.check()) {
    //    qCritical() << "Error::file check error";
    // //        emit checkFileResult(false);
    //    return false;
    // }
    // //    emit checkFileResult(true);
    return true;
}

bool BootMaker::install(const QString &image, const QString &unused_device, const QString &partition, bool formatDevice)
{
    qDebug() << "Starting installation - Image:" << image << "Partition:" << partition << "Format:" << formatDevice;

    if (m_pInstaller != nullptr) {
        emit m_usbDeviceMonitor->pauseMonitor();
        m_pInstaller->setImage(image);
        m_pInstaller->setPartionName(partition);
        m_pInstaller->setformat(formatDevice);
        m_pInstaller->beginInstall();
        emit m_usbDeviceMonitor->startMonitor();
    } else {
        qWarning() << "Installation failed - Installer not initialized";
    }

    return true;
}
