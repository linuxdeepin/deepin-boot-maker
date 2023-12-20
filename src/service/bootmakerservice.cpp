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

#if defined (Q_OS_LINUX) || defined (Q_OS_UNIX) ||  defined (Q_OS_MAC)
#include <polkit-qt5-1/PolkitQt1/Authority>
#include <polkit-qt5-1/PolkitQt1/Subject>
#endif

const QString s_PolkitAction = "com.deepin.bootmaker";

/**
   @brief Polkit action authorization check.
        Use com.deepin.bootmaker.policy config file.
        Default action id: "com.deepin.bootmaker"
   @note Available on linux/unix/macos platform.
   @return check passed.
 */
bool checkAuthorization(qint64 pid)
{
#if defined (Q_OS_LINUX) || defined (Q_OS_UNIX) ||  defined (Q_OS_MAC)
    PolkitQt1::Authority::Result ret = PolkitQt1::Authority::instance()->checkAuthorizationSync(
        s_PolkitAction, PolkitQt1::UnixProcessSubject(pid), PolkitQt1::Authority::AllowUserInteraction);
    if (PolkitQt1::Authority::Yes == ret) {
        return true;
    } else {
        qWarning() << qPrintable("Policy authorization check failed!");
        return false;
    }
#else
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
    QString execName;
    if (id > 0) {
        // Read contents of virtual /proc/{pid}/cmdline file
        QString exeSymlinkPath = QString("/proc/%1/exe").arg(id);
        char *actualpath = realpath(exeSymlinkPath.toStdString().c_str(), NULL);
        execName = QString(actualpath);
    }
    return execName;
}

BootMakerService::BootMakerService(QObject *parent) :
    QObject(parent), d_ptr(new BootMakerServicePrivate(this))
{
    Q_D(BootMakerService);
    d->bm = new BootMaker();
    QThread *bmThread = new QThread();
    d->bm->moveToThread(bmThread);
    bmThread->start();

    connect(d->bm, &BootMaker::removablePartitionsChanged,
    this, [ = ](const QList<DeviceInfo> &addlist, const QList<DeviceInfo>& dellist) {
        emit DeviceListChanged(deviceListToJson(addlist), deviceListToJson(dellist));
    });
    connect(d->bm, &BootMaker::finished,
            this, &BootMakerService::Finished);
//    connect(d->bm, &BootMaker::checkFileResult,
//            this, &BootMakerService::CheckFileResult);
    connect(d->bm, &BootMaker::reportProgress,
            this, &BootMakerService::ReportProgress);
    connect(d->bm, &BootMaker::reportProgress1,
            this, &BootMakerService::ReportProgress1);
//    connect(this, &BootMakerService::startInstall,
//            d->bm, &BootMaker::install);

    connect(d->bm, &BootMaker::finished,
    this, [ = ](int errcode, const QString &) {
        QThread::msleep(1000);
        qApp->exit(errcode);
    });

    connect(this, &BootMakerService::s_StartBootMarker,
            d->bm, &BootMaker::start);
}

BootMakerService::~BootMakerService()
{

}

void BootMakerService::Reboot()
{
    Q_D(BootMakerService);
    if (!d->checkCaller()) {
        return;
    }
    d->bm->reboot();
}

void BootMakerService::Start()
{
    Q_D(BootMakerService);
    if (!d->checkCaller()) {
        return;
    }

    emit s_StartBootMarker();
}

void BootMakerService::Stop()
{
    Q_D(BootMakerService);
    if (!d->checkCaller()) {
        return;
    }

    qDebug() << "service exit by call Stop";
    qApp->exit(0);
}

//!
//! \brief BootMakerService::DeviceList
//! \return
//! return json of devicelist
QString BootMakerService::DeviceList()
{
    qDebug() << "BootMakerService DeviceList";
    Q_D(BootMakerService);
    if (!d->checkCaller()) {
        return "";
    }
    return deviceListToJson(d->bm->deviceList());
}

bool BootMakerService::Install(const QString &image, const QString &device, const QString &partition,  bool formatDevice)
{
    Q_D(BootMakerService);
    if (!d->checkCaller()) {
        return false;
    }

    if (!d->disableCheck && !checkAuthorization(d->dbusCallerPid())) {
        return false;
    }

    qDebug() << "install  image:" << image << " device:" << device << " partition:" << partition;
    emit d->bm->startInstall(image, device, partition, formatDevice);
    return true;
}

bool BootMakerService::CheckFile(const QString &filepath)
{
    Q_D(BootMakerService);

    // if (!d->checkCaller()) {
    //     return false;
    // }
    return d->bm->checkfile(filepath);
//    emit d->bm->startCheckfile(filepath);
//    return true;
}

bool BootMakerServicePrivate::checkCaller()
{
    if (disableCheck) {
        return true;
    }

    Q_Q(BootMakerService);
    if (!q->calledFromDBus()) {
        return false;
    }

    qint64 callerPid = dbusCallerPid();
    QString callerExe = getProcIdExe(callerPid);
    QString dbmExe = QStandardPaths::findExecutable("deepin-boot-maker", {"/usr/bin"});

    qDebug() << "callerPid is: " << callerPid << "callerExe is:" << callerExe;

    if (callerExe != dbmExe) {
        qDebug() << QString("caller not authorized") ;
        return false;
    }
    qDebug() <<  QString("caller authorized");

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
        return 0;
    }

    auto interface = q->connection().interface();
    if (interface) {
        return static_cast<qint64>(interface->servicePid(q->message().service()).value());
    }

    return 0;
}
