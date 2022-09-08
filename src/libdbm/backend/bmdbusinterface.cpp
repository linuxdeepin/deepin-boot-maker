// SPDX-FileCopyrightText: 2016 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "bmdbusinterface.h"

/*
 * Implementation of interface class BMDBusInterface
 */

BMDBusInterface::BMDBusInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{

}

BMDBusInterface::~BMDBusInterface()
{
}

