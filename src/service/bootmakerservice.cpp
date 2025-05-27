// SPDX-FileCopyrightText: 2016 - 2022 UnionTech Software Technology Co., Ltd.
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
        Default action id: "com.deepin.bootmaker"
   @note Available on linux/unix/macos platform.
   @return check passed.
 */
bool checkAuthorization(qint64 pid, const QString &action)
{
#if defined (Q_OS_LINUX) || defined (Q_OS_UNIX) ||  defined (Q_OS_MAC)
    qDebug() << "Checking authorization for action:" << action << "PID:" << pid;
    PolkitQt1::Authority::Result ret = PolkitQt1::Authority::instance()->checkAuthorizationSync(
        action,
        PolkitQt1::UnixProcessSubject(pid),
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

#if 0 // Not use now
int getProcIdByExeName(std::string execName)
{
    int pid = -1;

    // Open the /proc directory
    DIR *dp = opendir("/proc");
    if (dp != NULL) {
        // Enumerate all entries in directory until process found
        struct dirent *dirp;
        while (pid < 0 && (dirp = readdir(dp))) {
            // Skip non-numeric entries
            int id = atoi(dirp->d_name);
            if (id > 0) {
                // Read contents of virtual /proc/{pid}/cmdline file
                auto exeSymlinkPath = std::string("/proc/") + dirp->d_name + "/exe";
                char *actualpath = realpath(exeSymlinkPath.c_str(), NULL);
                if (actualpath) {
                    // Compare against requested process name
                    if (execName == actualpath) {
                        pid = id;
                    }
                }
            }
        }
    }

    closedir(dp);

    return pid;
}
#endif

static QString getProcIdExe(qint64 id)
{
    qDebug() << "Getting executable path for process ID:" << id;
    QString execName;
    if (id > 0) {
        // Read contents of virtual /proc/{pid}/cmdline file
        QString exeSymlinkPath = QString("/proc/%1/exe").arg(id);
        char *actualpath = realpath(exeSymlinkPath.toStdString().c_str(), NULL);
        execName = QString(actualpath);
        qDebug() << "Process executable path:" << execName;
    }
    return execName;
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
    if (checkAuthorization(d->dbusCallerPid(), s_PolkitActionReboot)) {
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
    if (!d->checkCaller()) {
        qWarning() << "Start request denied - Invalid caller";
        return;
    }

    qDebug() << "Starting Boot Maker";
    emit s_StartBootMarker();
}

void BootMakerService::Stop()
{
    Q_D(BootMakerService);
    qInfo() << "Stop requested";
    if (!d->checkCaller()) {
        qWarning() << "Stop request denied - Invalid caller";
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
    if (!d->checkCaller()) {
        qWarning() << "Device list request denied - Invalid caller";
        return "";
    }
    return deviceListToJson(d->bm->deviceList());
}

bool BootMakerService::Install(const QString &image, const QString &device, const QString &partition, bool formatDevice)
{
    Q_D(BootMakerService);
    qInfo() << "Install requested - Image:" << image << "Device:" << device << "Partition:" << partition;
    
    if (!d->checkCaller()) {
        qWarning() << "Install request denied - Invalid caller";
        return false;
    }

    if (!d->disableCheck && !checkAuthorization(d->dbusCallerPid(), s_PolkitActionCreate)) {
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

bool BootMakerServicePrivate::checkCaller()
{
    if (disableCheck) {
        qDebug() << "Caller check disabled";
        return true;
    }

    Q_Q(BootMakerService);
    if (!q->calledFromDBus()) {
        qWarning() << "Caller check failed - Not called from DBus";
        return false;
    }

    qint64 callerPid = dbusCallerPid();
    QString callerExe = getProcIdExe(callerPid);
    QString dbmExe = QStandardPaths::findExecutable("deepin-boot-maker", {"/usr/bin"});

    qDebug() << "Caller check - PID:" << callerPid << "Executable:" << callerExe;

    if (callerExe != dbmExe) {
        qWarning() << "Caller not authorized - Invalid executable";
        return false;
    }
    qDebug() << "Caller authorized";
    return true;
}

/**
   @return DBus interface caller pid
    If the call is not from dbus (from UT), return 0
 */
qint64 BootMakerServicePrivate::dbusCallerPid()
{
    Q_Q(BootMakerService);
    if (!q->calledFromDBus()) {
        qDebug() << "Not called from DBus, returning 0";
        return 0;
    }

    auto interface = q->connection().interface();
    if (interface) {
        return static_cast<qint64>(interface->servicePid(q->message().service()).value());
    }

    qDebug() << "Failed to get DBus caller PID";
    return 0;
}
