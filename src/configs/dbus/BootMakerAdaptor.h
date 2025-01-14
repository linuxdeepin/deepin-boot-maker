// SPDX-FileCopyrightText: 2024 - 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef BOOTMAKERADAPTOR_H
#define BOOTMAKERADAPTOR_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
QT_BEGIN_NAMESPACE
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;
QT_END_NAMESPACE

/*
 * Adaptor class for interface com.deepin.bootmaker
 */
class BootMakerAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.bootmaker")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"com.deepin.bootmaker\">\n"
"    <signal name=\"DeviceListChanged\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"deviceListJson\"/>\n"
"    </signal>\n"
"    <signal name=\"Finished\">\n"
"      <arg direction=\"out\" type=\"i\" name=\"errcode\"/>\n"
"      <arg direction=\"out\" type=\"s\" name=\"description\"/>\n"
"    </signal>\n"
"    <signal name=\"ReportProgress\">\n"
"      <arg direction=\"out\" type=\"i\" name=\"current\"/>\n"
"      <arg direction=\"out\" type=\"i\" name=\"total\"/>\n"
"      <arg direction=\"out\" type=\"s\" name=\"title\"/>\n"
"      <arg direction=\"out\" type=\"s\" name=\"description\"/>\n"
"    </signal>\n"
"    <method name=\"DeviceList\">\n"
"      <arg direction=\"out\" type=\"s\"/>\n"
"    </method>\n"
"    <method name=\"Reboot\"/>\n"
"    <method name=\"Start\"/>\n"
"    <method name=\"Stop\"/>\n"
"    <method name=\"Install\">\n"
"      <arg direction=\"out\" type=\"b\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"image\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"device\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"partition\"/>\n"
"      <arg direction=\"in\" type=\"b\" name=\"formatDevice\"/>\n"
"    </method>\n"
"  </interface>\n"
        "")
public:
    BootMakerAdaptor(QObject *parent);
    virtual ~BootMakerAdaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    QString DeviceList();
    bool Install(const QString &image, const QString &device, const QString &partition, bool formatDevice);
    void Reboot();
    void Start();
    void Stop();
Q_SIGNALS: // SIGNALS
    void DeviceListChanged(const QString &deviceListJson);
    void Finished(int errcode, const QString &description);
    void ReportProgress(int current, int total, const QString &title, const QString &description);
};

#endif
