// SPDX-FileCopyrightText: 2016 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QObject>
#include <QString>
#include <QMap>

class DeviceInfo
{
public:
    DeviceInfo() {}
    DeviceInfo(const QString &path, quint64 used, quint64 total, const QString &label)
        : path(path),  used(used), total(total), label(label)
    {
    }

    QString path    = "";
    quint64 used    = 0;
    quint64 total   = 0;
    QString label   = QObject::tr("Removable Disk");
    QString uuid    = "";
    QString fstype  = "";
    QString target  = "";
    QString strDev = "";
    bool isDisk = false;
    bool needFormat = false;
    QMap<QString, DeviceInfo> children;

    bool operator==(const DeviceInfo& otherInfo)
    {
        return (this->path == otherInfo.path);
    }
};

Q_DECLARE_METATYPE(QList<DeviceInfo>);

QDataStream &operator<<(QDataStream &out, const DeviceInfo &msg);
QDataStream &operator>>(QDataStream &in, DeviceInfo &msg);

QString deviceListToJson(QList<DeviceInfo> deviceList);
QList<DeviceInfo> deviceListFromJson(QString json);
