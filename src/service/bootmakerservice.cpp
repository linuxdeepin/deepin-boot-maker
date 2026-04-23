// SPDX-FileCopyrightText: 2016 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "bootmakerservice.h"
#include "bootmakerservice_p.h"

#include <QDebug>
#include <QThread>
#include <QCoreApplication>
#include <QDBusContext>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QStandardPaths>

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <backend/bootmaker.h>


#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#if defined (Q_OS_LINUX) || defined (Q_OS_UNIX) ||  defined (Q_OS_MAC)
#include <polkit-qt5-1/PolkitQt1/Authority>
#include <polkit-qt5-1/PolkitQt1/Subject>
#endif
#else
#if defined (Q_OS_LINUX) || defined (Q_OS_UNIX) ||  defined (Q_OS_MAC)
#include <polkit-qt6-1/PolkitQt1/Authority>
#include <polkit-qt6-1/PolkitQt1/Subject>
#endif
#endif

const QString s_PolkitActionCreate = "com.deepin.bootmaker.create";
const QString s_PolkitActionReboot = "com.deepin.bootmaker.reboot";

/**
   @brief Polkit action authorization check.
        Use com.deepin.bootmaker.policy config file.
   @note Available on linux/unix/macos platform.
   @return check passed.
 */
bool checkAuthorization(const QString &busName, const QString &action)
{
#if defined (Q_OS_LINUX) || defined (Q_OS_UNIX) ||  defined (Q_OS_MAC)
    qDebug() << "Checking authorization for action:" << action << "BusName:" << busName;
    auto authority = PolkitQt1::Authority::instance();
    if (!authority) {
        qWarning() << "Failed to get Polkit authority instance";
        return false;
    }
    PolkitQt1::Authority::Result ret = authority->checkAuthorizationSync(
        action,
        PolkitQt1::SystemBusNameSubject(busName),
        PolkitQt1::Authority::AllowUserInteraction);

    if (PolkitQt1::Authority::Yes == ret) {
        qDebug() << "Authorization check passed for action:" << action;
        return true;
    } else {
        qWarning() << "Policy authorization check failed for action:" << action;
        return false;
    }
#else
    qDebug() << "Authorization check skipped on non-Linux/Unix/Mac platform";
    return true;
#endif
}

BootMakerService::BootMakerService(QObject *parent) :
    QObject(parent), d_ptr(new BootMakerServicePrivate(this))
{
    Q_D(BootMakerService);
    qDebug() << "Initializing Boot Maker Service";
    
    d->bm = new BootMaker();
    QThread *bmThread = new QThread();
    d->bm->moveToThread(bmThread);
    bmThread->start();
    qDebug() << "Boot Maker handler moved to worker thread";

    connect(d->bm, &BootMaker::removablePartitionsChanged,
    this, [ = ](const QList<DeviceInfo> &addlist, const QList<DeviceInfo>& dellist) {
        qDebug() << "Device list changed - Added:" << addlist.size() << "Removed:" << dellist.size();
        emit DeviceListChanged(deviceListToJson(addlist), deviceListToJson(dellist));
    });
    connect(d->bm, &BootMaker::finished,
            this, &BootMakerService::Finished);
    connect(d->bm, &BootMaker::reportProgress,
            this, &BootMakerService::ReportProgress);
    connect(d->bm, &BootMaker::reportProgress1,
            this, &BootMakerService::ReportProgress1);

    connect(d->bm, &BootMaker::finished,
    this, [ = ](int errcode, const QString &) {
        qDebug() << "Boot Maker finished with error code:" << errcode;
        QThread::msleep(1000);
        qApp->exit(errcode);
    });

    connect(this, &BootMakerService::s_StartBootMarker,
            d->bm, &BootMaker::start);
    qDebug() << "Boot Maker Service initialization completed";
}

BootMakerService::~BootMakerService()
{
    qDebug() << "Destroying Boot Maker Service";
}

void BootMakerService::Reboot()
{
    Q_D(BootMakerService);
    qInfo() << "Reboot requested";
    if (d->checkAuthorization(s_PolkitActionReboot)) {
        qDebug() << "Reboot authorized, proceeding";
        d->bm->reboot();
    } else {
        qWarning() << "Reboot request denied - Authorization failed";
    }
}

void BootMakerService::Start()
{
    Q_D(BootMakerService);
    qInfo() << "Start requested";
    if (!d->checkAuthorization(s_PolkitActionCreate)) {
        qWarning() << "Start request denied - Authorization failed";
        return;
    }

    qDebug() << "Starting Boot Maker";
    emit s_StartBootMarker();
}

void BootMakerService::Stop()
{
    Q_D(BootMakerService);
    qInfo() << "Stop requested";
    if (!d->checkAuthorization(s_PolkitActionCreate)) {
        qWarning() << "Stop request denied - Authorization failed";
        return;
    }

    qDebug() << "Stopping Boot Maker Service";
    qApp->exit(0);
}

//!
//! \brief BootMakerService::DeviceList
//! \return
//! return json of devicelist
QString BootMakerService::DeviceList()
{
    Q_D(BootMakerService);
    qDebug() << "Device list requested";
    if (!d->checkAuthorization(s_PolkitActionCreate)) {
        qWarning() << "Device list request denied - Authorization failed";
        return "";
    }
    return deviceListToJson(d->bm->deviceList());
}

bool BootMakerService::Install(const QString &image, const QString &device, const QString &partition, bool formatDevice)
{
    Q_D(BootMakerService);
    qInfo() << "Install requested - Image:" << image << "Device:" << device << "Partition:" << partition;
    
    if (!d->checkAuthorization(s_PolkitActionCreate)) {
        qWarning() << "Install request denied - Authorization failed";
        return false;
    }

    qDebug() << "Starting installation process";
    emit d->bm->startInstall(image, device, partition, formatDevice);
    return true;
}

bool BootMakerService::CheckFile(const QString &filepath)
{
    Q_D(BootMakerService);
    qDebug() << "File check requested:" << filepath;
    return d->bm->checkfile(filepath);
}

bool BootMakerServicePrivate::checkAuthorization(const QString &action)
{
    if (disableCheck) {
        qDebug() << "Authorization check disabled";
        return true;
    }

    Q_Q(BootMakerService);
    if (!q->calledFromDBus()) {
        qWarning() << "Authorization check failed - Not called from DBus";
        return false;
    }

    QString busName = q->message().service();
    bool ret = ::checkAuthorization(busName, action);
    qDebug() << "Authorization check result: " << ret;

    return ret;
}
