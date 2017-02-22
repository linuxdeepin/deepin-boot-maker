/**
 * Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#pragma once

#include <QObject>
#include <QString>

class DeviceInfo
{
public:
    DeviceInfo(){}
    DeviceInfo(const QString &path, quint32 used, quint32 total, const QString &label)
        : path(path),  used(used), total(total), label(label)
    {
    }

    QString path    = "";
    quint32 used    = 0;
    quint32 total   = 0;
    QString label   = QObject::tr("Removable Disk");
    QString uuid    = "";
    QString target  = "";
};

Q_DECLARE_METATYPE(QList<DeviceInfo>);

QDataStream &operator<<(QDataStream &out, const DeviceInfo &msg);
QDataStream &operator>>(QDataStream &in, DeviceInfo &msg);

QString deviceListToJson(QList<DeviceInfo> deviceList);
QList<DeviceInfo> deviceListFromJson(QString json);
