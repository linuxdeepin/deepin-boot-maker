/**
 * Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "bminterface.h"

#include <QDebug>

#include "backend/bmhandler.h"

#ifdef Q_OS_LINUX
#include "backend/bmdbushandler.h"
#else
#include "backend/bootmaker.h"
#endif

class BMInterfacePrivate
{
public:
    BMInterfacePrivate(BMInterface *parent) : q_ptr(parent) {}

    BMHandler       *handler = nullptr;

    BMInterface *q_ptr;
    Q_DECLARE_PUBLIC(BMInterface)
};

BMInterface::BMInterface(QObject *parent) :
    QObject(parent), d_ptr(new BMInterfacePrivate(this))
{
    Q_D(BMInterface);
#ifdef Q_OS_LINUX
    d->handler = new BMDbusHandler;
#else
    d->handler = new BootMaker;
#endif
    connect(d->handler, &BMHandler::removablePartitionsChanged,
            this, &BMInterface::deviceListChanged);
    connect(d->handler, &BMHandler::finished,
            this, &BMInterface::finished);
    connect(d->handler, &BMHandler::reportProgress ,
            this, &BMInterface::reportProgress);
}

BMInterface::~BMInterface()
{

}

void BMInterface::start()
{
    Q_D(BMInterface);
    return d->handler->start();
}

void BMInterface::stop()
{
    Q_D(BMInterface);
    return d->handler->stop();
}

void BMInterface::reboot()
{
    Q_D(BMInterface);
    return d->handler->reboot();
}

QList<DeviceInfo> BMInterface::deviceList()
{
    Q_D(BMInterface);
    return d->handler->deviceList();
}

bool BMInterface::install(const QString &image, const QString &device, const QString &partition, bool formatDevice)
{
    Q_D(BMInterface);
    emit d->handler->reportProgress(5, 0, "init", "");
    qDebug() << "start BMInterface::install";
    return d->handler->install(image,device, partition, formatDevice);
}
