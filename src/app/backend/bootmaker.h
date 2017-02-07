#pragma once

#include "bmhandler.h"

class DeviceMonitor;
class BootMaker : public BMHandler
{
    Q_OBJECT
public:
    explicit BootMaker(QObject *parent = 0);

public slots:
    void start();
    void stop();
    const QList<DeviceInfo> deviceList() const;
    bool install(const QString &image,
                 const QString &device,
                 const QString &partition,
                 bool  formatDevice);

private:
    DeviceMonitor        *m_usbDeviceMonitor = nullptr;
};

