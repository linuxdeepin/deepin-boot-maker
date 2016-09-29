#include "usbdevicemonitor.h"

#include <QThread>
#include <QDataStream>

#include "utils.h"

UsbDeviceMonitor::UsbDeviceMonitor(QObject *parent) : QObject(parent)
{
    m_timer = new QTimer(this);

    m_timer->setInterval(3000);

    connect(m_timer, &QTimer::timeout, this, [ = ] {
        QList<DeviceInfo> list = Utils::ListUsbDrives();
        emit this->removePartitionsChanged(list);
    });
}

void UsbDeviceMonitor::pauseMonitor()
{
    m_timer->stop();
}

void UsbDeviceMonitor::startMonitor()
{
    m_timer->start();
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
