// SPDX-FileCopyrightText: 2024 - 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "BootMakerAdaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class BootMakerAdaptor
 */

BootMakerAdaptor::BootMakerAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

BootMakerAdaptor::~BootMakerAdaptor()
{
    // destructor
}

QString BootMakerAdaptor::DeviceList()
{
    // handle method call com.deepin.bootmaker.DeviceList
    QString out0;
    QMetaObject::invokeMethod(parent(), "DeviceList", Q_RETURN_ARG(QString, out0));
    return out0;
}

bool BootMakerAdaptor::Install(const QString &image, const QString &device, const QString &partition, bool formatDevice)
{
    // handle method call com.deepin.bootmaker.Install
    bool out0;
    QMetaObject::invokeMethod(parent(), "Install", Q_RETURN_ARG(bool, out0), Q_ARG(QString, image), Q_ARG(QString, device), Q_ARG(QString, partition), Q_ARG(bool, formatDevice));
    return out0;
}

void BootMakerAdaptor::Reboot()
{
    // handle method call com.deepin.bootmaker.Reboot
    QMetaObject::invokeMethod(parent(), "Reboot");
}

void BootMakerAdaptor::Start()
{
    // handle method call com.deepin.bootmaker.Start
    QMetaObject::invokeMethod(parent(), "Start");
}

void BootMakerAdaptor::Stop()
{
    // handle method call com.deepin.bootmaker.Stop
    QMetaObject::invokeMethod(parent(), "Stop");
}

