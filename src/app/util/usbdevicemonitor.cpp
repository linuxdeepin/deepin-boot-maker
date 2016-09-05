#include "usbdevicemonitor.h"

#include <QThread>
#include <QDataStream>

#include "utils.h"

UsbDeviceMonitor::UsbDeviceMonitor(QObject *parent) : QObject(parent)
{

}

void UsbDeviceMonitor::run()
{
    while (true) {
        QThread::sleep(5);
        QList<DeviceInfo> list;
        emit removePartitionsChanged(list);
    }
}


QDataStream &operator<<(QDataStream &out, const DeviceInfo &msg)
{
    out << msg.used << msg.total << msg.name << msg.device;
    return out;
}
QDataStream &operator>>(QDataStream &in, DeviceInfo &msg)
{
    quint32 used;
    quint32 total;
    QString name;
    QString device;
    in >> used >> total >> name >> device;
    msg = DeviceInfo(used, total, name, device);
    return in;
}
