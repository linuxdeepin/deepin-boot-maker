/*
 * Copyright (C) 2017 ~ 2018 Wuhan Deepin Technology Co., Ltd.
 *
 * Author:     Iceyer <me@iceyer.net>
 *
 * Maintainer: Iceyer <me@iceyer.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
    qRegisterMetaType<QList<DeviceInfo>>();

    m_timer = new QTimer(this);
    m_timer->setInterval(2000);
    connect(m_timer, &QTimer::timeout, this, [ = ] {
        QList<DeviceInfo> list = Utils::ListUsbDrives();
        qDebug() << "list length:" << list.length();
        for (int i = 0; i < list.size(); i++)
        {
            qDebug() << "list i:" << i << " path:" << list.at(i).path << " label:" << list.at(i).label;
        }

        QList<DeviceInfo> intersectList = this->getIntersectDevice(list);
        qDebug() << "intersectlist count:" << intersectList.size();
        QList<DeviceInfo> addList = this->getNorDevice(list, intersectList);
        QList<DeviceInfo> delList = this->getNorDevice(m_deviceList, intersectList);

        if ((!addList.isEmpty()) || !delList.isEmpty()) {
            emit this->removablePartitionsChanged(addList, delList);
            qDebug() << "addlist count = " << addList.count();
            qDebug() << "reducelist count =" << delList.count();
        }

        this->m_deviceList = list;
    });

    connect(this, &DeviceMonitor::pauseMonitor, this, [ = ]() {
        m_timer->stop();
    });
    connect(this, &DeviceMonitor::startMonitor, this, [ = ]() {
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
           XorList.push_back(info);
       }
   }

   return XorList;
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
        obj.insert("fstype", device.fstype);
        obj.insert("needformat", device.needFormat);
        obj.insert("strDev", device.strDev);
        obj.insert("isDisk", device.isDisk);
        qDebug() << device.needFormat;
        array.push_back(obj);
    }

//    QJsonObject obj;
//    obj.insert("path", "Utest");
//    obj.insert("label", "U盘test");
//    obj.insert("used", 10000);
//    obj.insert("total", 100000);
//    obj.insert("needformat", false);
//    array.push_back(obj);
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
        device.fstype = obj.value("fstype").toString();
        device.needFormat = obj.value("needformat").toBool();
        device.isDisk = obj.value("isDisk").toBool();
        device.strDev = obj.value("strDev").toString();
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
