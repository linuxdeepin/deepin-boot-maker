#pragma once

#include <QObject>
#include <QTimer>

#include "deviceinfo.h"

class DeviceMonitor : public QObject
{
    Q_OBJECT
public:
    explicit DeviceMonitor(QObject *parent = 0);

signals:
    void startMonitor();
    void pauseMonitor();
    void removablePartitionsChanged(const QList<DeviceInfo> &list);

public slots:
    const QList<DeviceInfo> deviceList() const;

private:
    QTimer *m_timer = nullptr;
};
