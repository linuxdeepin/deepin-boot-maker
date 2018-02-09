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

#pragma once

#include "bmhandler.h"
#include "bmdbusinterface.h"
#include "../util/deviceinfo.h"
#include <QDebug>
#include <QDBusConnection>

class DeviceMonitor;
class BMDbusHandler : public BMHandler
{
    Q_OBJECT
public:
    explicit BMDbusHandler(QObject *parent = 0)
        : BMHandler(parent)
    {
        m_dbus = new BMDBusInterface("com.deepin.bootmaker",
                                     "/com/deepin/bootmaker",
                                     QDBusConnection::systemBus(),
                                     this);

        connect(m_dbus, &BMDBusInterface::DeviceListChanged,
        this, [ = ](const QString & deviceListJson) {
            emit removablePartitionsChanged(deviceListFromJson(deviceListJson));
        });
        connect(m_dbus, &BMDBusInterface::Finished,
                this, &BMDbusHandler::finished);
        connect(m_dbus, &BMDBusInterface::ReportProgress ,
                this, &BMDbusHandler::reportProgress);
    }

public slots:
    void reboot()
    {
        m_dbus->Reboot();
    }

    void start()
    {
        m_dbus->Start();
    }

    void stop()
    {
        m_dbus->Stop();
    }

    const QList<DeviceInfo> deviceList() const
    {
        return  deviceListFromJson(m_dbus->DeviceList());
    }

    bool install(const QString &image,
                 const QString &device,
                 const QString &partition,
                 bool  formatDevice)
    {
        return m_dbus->Install(image, device, partition, formatDevice);
    }

private:
    BMDBusInterface        *m_dbus = nullptr;
};
