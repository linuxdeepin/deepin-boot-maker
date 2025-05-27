// SPDX-FileCopyrightText: 2016 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

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

    qDebug() << "Initializing Boot Maker Interface";
    qRegisterMetaType<QList<DeviceInfo>>();

#ifdef Q_OS_LINUX
    qDebug() << "Creating D-Bus handler for Linux platform";
    d->handler = new BMDbusHandler;
#else
    qDebug() << "Creating Boot Maker handler for non-Linux platform";
    d->handler = new BootMaker;
#endif

    qDebug() << "Moving handler to worker thread";
    QThread *handlerwork = new QThread;
    d->handler->moveToThread(handlerwork);
    handlerwork->start();

    qDebug() << "Setting up signal connections";
    connect(d->handler, &BMHandler::removablePartitionsChanged,
            this, &BMInterface::deviceListChanged);
    connect(d->handler, &BMHandler::finished,
            this, &BMInterface::finished);
//    connect(d->handler, &BMHandler::checkFileResult,
//            this, &BMInterface::checkFileResult);
    connect(d->handler, &BMHandler::reportProgress,
            this, &BMInterface::reportProgress);
    connect(d->handler,&BMHandler::reportProgress1,
            this,&BMInterface::reportProgress1);
    connect(this, &BMInterface::startInstall,
            d->handler, &BMHandler::startInstall );
    connect(d->handler, &BMHandler::startInstallRet,
            this, &BMInterface::startInstallRet);
    qDebug() << "Boot Maker Interface initialization completed";
}

BMInterface::~BMInterface()
{
    qDebug() << "Destroying Boot Maker Interface";
}

void BMInterface::start()
{
    Q_D(BMInterface);
    qInfo() << "Starting Boot Maker handler";
    return d->handler->start();
}

void BMInterface::stop()
{
    Q_D(BMInterface);
    qInfo() << "Stopping Boot Maker handler";
    return d->handler->stop();
}

void BMInterface::reboot()
{
    Q_D(BMInterface);
    qInfo() << "Initiating system reboot";
    return d->handler->reboot();
}

QList<DeviceInfo> BMInterface::deviceList()
{
    qDebug() << "Retrieving device list";
    Q_D(BMInterface);
    return d->handler->deviceList();
}

bool BMInterface::checkfile(const QString &filepath)
{
    Q_D(BMInterface);
    return d->handler->checkfile(filepath);
}

//bool BMInterface::install(const QString &image, const QString &device, const QString &partition, bool formatDevice)
//{
//    Q_D(BMInterface);
//    emit d->handler->reportProgress(5, 0, "init", "");
//    qDebug() << "start BMInterface::install";
//    emit d->handler->startInstall(image,device, partition, formatDevice);
//    return true;
//}
