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

#include "bminterface.h"

#include <QDebug>
#include <QThread>


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

    qRegisterMetaType<QList<DeviceInfo>>();

#ifdef Q_OS_LINUX
    d->handler = new BMDbusHandler;
#else
    d->handler = new BootMaker;
#endif
    QThread *handlerwork = new QThread;
    d->handler->moveToThread(handlerwork);
    handlerwork->start();

    connect(d->handler, &BMHandler::removablePartitionsChanged,
            this, &BMInterface::deviceListChanged);
    connect(d->handler, &BMHandler::finished,
            this, &BMInterface::finished);
    connect(d->handler, &BMHandler::reportProgress ,
            this, &BMInterface::reportProgress);

    connect(            this, &BMInterface::startInstall,
                        d->handler, &BMHandler::startInstall );
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

//bool BMInterface::install(const QString &image, const QString &device, const QString &partition, bool formatDevice)
//{
//    Q_D(BMInterface);
//    emit d->handler->reportProgress(5, 0, "init", "");
//    qDebug() << "start BMInterface::install";
//    emit d->handler->startInstall(image,device, partition, formatDevice);
//    return true;
//}
