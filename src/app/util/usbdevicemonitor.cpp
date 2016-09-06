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
        QThread::sleep(3);
        QList<DeviceInfo> list = Utils::ListUsbDrives();
        emit removePartitionsChanged(list);
    }
}


QDataStream &operator<<(QDataStream &out, const DeviceInfo &msg)
{
    out << msg.path << msg.used << msg.total <<  msg.label;
    return out;
}
QDataStream &operator>>(QDataStream &in, DeviceInfo &msg)
{
    QString path;
    quint32 used;
    quint32 total;
    QString label;
    in >> path >> used >> total >> label;
    msg = DeviceInfo(path, used, total, label);
    return in;
}
