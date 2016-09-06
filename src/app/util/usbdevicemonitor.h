#ifndef USBDEVICEMONITOR_H
#define USBDEVICEMONITOR_H

#include <QObject>

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

Q_DECLARE_METATYPE(QList<DeviceInfo>);

#endif // USBDEVICEMONITOR_H
