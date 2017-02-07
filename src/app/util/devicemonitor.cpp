#include "devicemonitor.h"

#include <QThread>
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "utils.h"

DeviceMonitor::DeviceMonitor(QObject *parent) : QObject(parent)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(3000);
    connect(m_timer, &QTimer::timeout, this, [ = ] {
        QList<DeviceInfo> list = Utils::ListUsbDrives();
        emit this->removablePartitionsChanged(list);
    });

    connect(this, &DeviceMonitor::pauseMonitor, this, [ = ]() {
        m_timer->stop();
    });
    connect(this, &DeviceMonitor::startMonitor, this, [ = ]() {
        m_timer->start();
    });
}

const QList<DeviceInfo> DeviceMonitor::deviceList() const
{
    return Utils::ListUsbDrives();
}

QString deviceListToJson(QList<DeviceInfo> deviceList)
{
    QJsonArray array;
    for (auto device : deviceList) {
        QJsonObject obj;
        obj.insert("path", device.path);
        obj.insert("label", device.label);
        obj.insert("used", static_cast<int>(device.used));
        obj.insert("total", static_cast<int>(device.total));
        array.push_back(obj);
    }

    QJsonDocument doc;
    doc.setArray(array);
    return QString::fromUtf8(doc.toJson());
}

QList<DeviceInfo> deviceListFromJson(QString json)
{
    QList<DeviceInfo> list;
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    for (auto jsonObj : doc.array()) {
        DeviceInfo device;
        auto obj = jsonObj.toObject();
        device.path = obj.value("path").toString();
        device.label = obj.value("label").toString();
        device.used = static_cast<quint32>(obj.value("used").toInt());
        device.total = static_cast<quint32>(obj.value("total").toInt());
        list.push_back(device);
    }
    return list;
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
