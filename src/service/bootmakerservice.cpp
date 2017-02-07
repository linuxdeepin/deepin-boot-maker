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
            this, [=](int errcode, const QString &){
        QThread::msleep(1000);
        qApp->exit(errcode);
    });
}

BootMakerService::~BootMakerService()
{

}

void BootMakerService::Start()
{
    Q_D(BootMakerService);
    d->bm->start();
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
