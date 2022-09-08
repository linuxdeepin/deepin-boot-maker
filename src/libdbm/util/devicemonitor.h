// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once
#include "deviceinfo.h"

#include <QObject>
#include <QTimer>

class DeviceMonitor : public QObject
{
    Q_OBJECT
public:
    explicit DeviceMonitor(QObject *parent = nullptr);
    ~DeviceMonitor() {

    }

private:
    QList<DeviceInfo> getIntersectDevice(const QList<DeviceInfo>& list);
    static QList<DeviceInfo> getNorDevice(const QList<DeviceInfo>& calcuList, const QList<DeviceInfo>& refList);

signals:
    void startMonitor();
    void pauseMonitor();
    void removablePartitionsChanged(const QList<DeviceInfo> &addlist, const QList<DeviceInfo>& reducelist);

public slots:
    const QList<DeviceInfo> deviceList() const;

private:
    QTimer *m_timer = nullptr;
    QList<DeviceInfo> m_deviceList;
};
