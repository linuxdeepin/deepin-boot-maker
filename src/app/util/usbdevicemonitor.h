#ifndef USBDEVICEMONITOR_H
#define USBDEVICEMONITOR_H

#include <QObject>

class DeviceInfo
{
public:
    DeviceInfo(quint32 used, quint32 total, const QString &name, const QString &device)
        : used(used), total(total), name(name), device(device)
    {
    }

    quint32 used;
    quint32 total;
    QString name;
    QString device;
};

QDataStream &operator<<(QDataStream &out, const DeviceInfo &msg);
QDataStream &operator>>(QDataStream &in, DeviceInfo &msg);

class UsbDeviceMonitor : public QObject
{
    Q_OBJECT
public:
    explicit UsbDeviceMonitor(QObject *parent = 0);

signals:
    void removePartitionsChanged(const QList<DeviceInfo> &list);

public slots:
    void run();
};

#endif // USBDEVICEMONITOR_H
