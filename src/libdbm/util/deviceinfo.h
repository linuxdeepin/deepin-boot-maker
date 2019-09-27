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

#include <QObject>
#include <QString>
#include <QMap>

class DeviceInfo
{
public:
    DeviceInfo() {}
    DeviceInfo(const QString &path, quint32 used, quint32 total, const QString &label)
        : path(path),  used(used), total(total), label(label)
    {
    }

    QString path    = "";
    quint32 used    = 0;
    quint32 total   = 0;
    QString label   = QObject::tr("Removable Disk");
    QString uuid    = "";
    QString fstype  = "";
    QString target  = "";
    bool needFormat = false;
    QMap<QString, DeviceInfo> children;
};

Q_DECLARE_METATYPE(QList<DeviceInfo>);

QDataStream &operator<<(QDataStream &out, const DeviceInfo &msg);
QDataStream &operator>>(QDataStream &in, DeviceInfo &msg);

QString deviceListToJson(QList<DeviceInfo> deviceList);
QList<DeviceInfo> deviceListFromJson(QString json);
