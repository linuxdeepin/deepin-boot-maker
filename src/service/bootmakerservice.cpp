/**
 * Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "bootmakerservice.h"

#include <QDebug>
#include <QThread>
#include <QCoreApplication>

#include "../app/backend/bootmaker.h"

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int getProcIdByName(std::string procName)
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
                auto cmdPath = std::string("/proc/") + dirp->d_name + "/cmdline";
                std::ifstream cmdFile(cmdPath.c_str());
                std::string cmdLine;
                getline(cmdFile, cmdLine);
                if (!cmdLine.empty()) {
                    // Keep first cmdline item which contains the program path
                    size_t pos = cmdLine.find('\0');
                    if (pos != std::string::npos) {
                        cmdLine = cmdLine.substr(0, pos);
                    }
                    // Keep program name only, removing the path
                    pos = cmdLine.rfind('/');
                    if (pos != std::string::npos) {
                        cmdLine = cmdLine.substr(pos + 1);
                    }
                    // Compare against requested process name
                    if (procName == cmdLine) {
                        pid = id;
                    }
                }
            }
        }
    }

    closedir(dp);

    return pid;
}

class BootMakerServicePrivate
{
public:
    BootMakerServicePrivate(BootMakerService *parent) : q_ptr(parent) {}

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
        qDebug() << "seend ----------------------";
        emit DeviceListChanged(deviceListToJson(list));
    });
    connect(d->bm, &BootMaker::finished,
            this, &BootMakerService::Finished);
    connect(d->bm, &BootMaker::reportProgress,
            this, &BootMakerService::ReportProgress);

    connect(this, &BootMakerService::startInstall,
            d->bm, &BootMaker::install);

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
    d->bm->reboot();
}

#include <PolkitQt1/Authority>

void BootMakerService::Start()
{
    Q_D(BootMakerService);

    auto pid = getProcIdByName("deepin-boot-maker-gui");

    qDebug() << pid;
    PolkitQt1::Authority::Result result;
    PolkitQt1::UnixProcessSubject process(21340);
    result = PolkitQt1::Authority::instance()->checkAuthorizationSync("com.deepin.bootmaker",
             process,
             PolkitQt1::Authority::None);
    if (result == PolkitQt1::Authority::Yes) {
        qDebug() <<  QString("authorized");
    } else {
        qDebug() << QString("Not authorized") << result;
        return;
    }

//    d->bm->start();
}

void BootMakerService::Stop()
{
    qApp->exit(0);
}

//!
//! \brief BootMakerService::DeviceList
//! \return
//! return json of devicelist
QString BootMakerService::DeviceList()
{
    Q_D(BootMakerService);
    return deviceListToJson(d->bm->deviceList());
}

bool BootMakerService::Install(const QString &image, const QString &device, const QString &partition, bool formatDevice)
{
    emit startInstall(image, device, partition, formatDevice);
    return true;
}
