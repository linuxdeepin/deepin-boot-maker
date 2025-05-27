// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "devicemonitor.h"

#include "utils.h"

#include <QDebug>
#include <QThread>
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>

//#pragma execution_character_set("utf-8")

DeviceMonitor::DeviceMonitor(QObject *parent) : QObject(parent)
{
    qDebug() << "Initializing device monitor";
    qRegisterMetaType<QList<DeviceInfo>>();

    m_timer = new QTimer(this);
    m_timer->setInterval(2000);
    connect(m_timer, &QTimer::timeout, this, [ = ] {
        QList<DeviceInfo> list = Utils::ListUsbDrives();
        qDebug() << "Detected" << list.length() << "USB devices";
        for (int i = 0; i < list.size(); i++)
        {
            qDebug() << "Device" << i << "- Path:" << list.at(i).path 
                     << "Label:" << list.at(i).label 
                     << "Type:" << list.at(i).fstype
                     << "Format needed:" << list.at(i).needFormat;
        }

        QList<DeviceInfo> intersectList = this->getIntersectDevice(list);
        qDebug() << "Found" << intersectList.size() << "unchanged devices";
        QList<DeviceInfo> addList = this->getNorDevice(list, intersectList);
        QList<DeviceInfo> delList = this->getNorDevice(m_deviceList, intersectList);

        if ((!addList.isEmpty()) || !delList.isEmpty()) {
            qInfo() << "Device changes detected - Added:" << addList.count() 
                    << "Removed:" << delList.count();
            emit this->removablePartitionsChanged(addList, delList);
        }

        this->m_deviceList = list;
    });

    connect(this, &DeviceMonitor::pauseMonitor, m_timer, &QTimer::stop);
    connect(this, &DeviceMonitor::startMonitor, this, [ = ]() {
        qInfo() << "Starting device monitoring";
        this->m_deviceList.clear();
        m_timer->start();
    });
}

QList<DeviceInfo> DeviceMonitor::getIntersectDevice(const QList<DeviceInfo>& list)
{
   QList<DeviceInfo> intersectList;

   foreach (DeviceInfo info, list) {
       foreach (DeviceInfo tempInfo, m_deviceList) {
           if ((tempInfo == info)&&(tempInfo.total == info.total)&&(tempInfo.used == info.used)) {
               qDebug() << "Found unchanged device - Path:" << info.path 
                        << "Label:" << info.label;
               intersectList.push_back(info);
           }
       }
   }

   return intersectList;
}

QList<DeviceInfo> DeviceMonitor::getNorDevice(const QList<DeviceInfo>& calcuList, const QList<DeviceInfo>& refList)
{
   QList<DeviceInfo> XorList;

   foreach (DeviceInfo info, calcuList) {
       bool bInsert = true;

       foreach (DeviceInfo tempInfo, refList) {
           if (info.path == tempInfo.path) {
               bInsert = false;
               break;
           }
       }

       if (bInsert) {
           qDebug() << "Found new device - Path:" << info.path 
                    << "Label:" << info.label;
           XorList.push_back(info);
       }
   }

   return XorList;
}

const QList<DeviceInfo> DeviceMonitor::deviceList() const
{
    qDebug() << "Retrieving current device list";
    return Utils::ListUsbDrives();
}

QString deviceListToJson(QList<DeviceInfo> deviceList)
{
    qDebug() << "Converting device list to JSON format";
    QJsonArray array;
    for (auto device : deviceList) {
        QJsonObject obj;
        obj.insert("path", device.path);
        obj.insert("label", device.label);
        obj.insert("used", static_cast<int>(device.used));
        obj.insert("total", static_cast<int>(device.total));
        obj.insert("fstype", device.fstype);
        obj.insert("needformat", device.needFormat);
        obj.insert("strDev", device.strDev);
        obj.insert("isDisk", device.isDisk);
        qDebug() << "Device format needed:" << device.needFormat;
        array.push_back(obj);
    }

    QJsonDocument doc;
    doc.setArray(array);
    return QString::fromUtf8(doc.toJson());
}

QList<DeviceInfo> deviceListFromJson(QString json)
{
    qDebug() << "Parsing device list from JSON";
    QList<DeviceInfo> list;
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    for (auto jsonObj : doc.array()) {
        DeviceInfo device;
        auto obj = jsonObj.toObject();
        device.path = obj.value("path").toString();
        device.label = obj.value("label").toString();
        device.used = static_cast<quint32>(obj.value("used").toInt());
        device.total = static_cast<quint32>(obj.value("total").toInt());
        device.fstype = obj.value("fstype").toString();
        device.needFormat = obj.value("needformat").toBool();
        device.isDisk = obj.value("isDisk").toBool();
        device.strDev = obj.value("strDev").toString();
        qDebug() << "Parsed device - Path:" << device.path 
                 << "Label:" << device.label;
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
