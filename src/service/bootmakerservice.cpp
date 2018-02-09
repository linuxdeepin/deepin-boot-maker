/*
 * Copyright (C) 2016 ~ 2018 Wuhan Deepin Technology Co., Ltd.
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

#include "bootmakerservice.h"

#include <QDebug>
#include <QThread>
#include <QCoreApplication>

#include <QDBusContext>
#include <QDBusConnection>
#include <QDBusConnectionInterface>

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <backend/bootmaker.h>

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

static std::string getProcIdExe(int id)
{
    std::string execName;
    if (id > 0) {
        // Read contents of virtual /proc/{pid}/cmdline file
        auto exeSymlinkPath = std::string("/proc/") + QString("%1").arg(id).toStdString() + "/exe";
        char *actualpath = realpath(exeSymlinkPath.c_str(), NULL);
        execName = std::string(actualpath);
    }
    return execName;
}

class BootMakerServicePrivate
{
public:
    BootMakerServicePrivate(BootMakerService *parent) : q_ptr(parent) {}

    bool checkCaller();

    BootMaker   *bm;

    BootMakerService *q_ptr;
    Q_DECLARE_PUBLIC(BootMakerService)
};

BootMakerService::BootMakerService(QObject *parent) :
    QObject(parent), d_ptr(new BootMakerServicePrivate(this))
{
    Q_D(BootMakerService);
    d->bm = new BootMaker;
    QThread *bmThread = new QThread;
    d->bm->moveToThread(bmThread);
    bmThread->start();

    connect(d->bm, &BootMaker::removablePartitionsChanged,
    this, [ = ](const QList<DeviceInfo> &list) {
        emit DeviceListChanged(deviceListToJson(list));
    });
    connect(d->bm, &BootMaker::finished,
            this, &BootMakerService::Finished);
    connect(d->bm, &BootMaker::reportProgress,
            this, &BootMakerService::ReportProgress);

//    connect(this, &BootMakerService::startInstall,
//            d->bm, &BootMaker::install);

    connect(d->bm, &BootMaker::finished,
    this, [ = ](int errcode, const QString &) {
        QThread::msleep(1000);
        qApp->exit(errcode);
    });
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

    d->bm->start();
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
    Q_D(BootMakerService);
    if (!d->checkCaller()) {
        return "";
    }
    return deviceListToJson(d->bm->deviceList());
}

bool BootMakerService::Install(const QString &image, const QString &device, const QString &partition, bool formatDevice)
{
    Q_D(BootMakerService);
    if (!d->checkCaller()) {
        return false;
    }
    emit d->bm->startInstall(image, device, partition, formatDevice);
    return true;
}

bool BootMakerServicePrivate::checkCaller()
{
    Q_Q(BootMakerService);

    auto callerPid = static_cast<int>(q->connection().interface()->servicePid(q->message().service()).value());
    auto callerExe = getProcIdExe(callerPid);
    auto dbmExe = "/usr/bin/deepin-boot-maker";
//    auto dbmPid = getProcIdByExeName();

    qDebug() << "callerPid is: " << callerPid  << "callerExe is:" << callerExe.c_str() ;

    if (callerExe != dbmExe) {
        qDebug() << QString("caller not authorized") ;
        return false;
    }
    qDebug() <<  QString("caller authorized");
    return true;
}
