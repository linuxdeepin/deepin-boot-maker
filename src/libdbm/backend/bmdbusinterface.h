// SPDX-FileCopyrightText: 2016 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#ifndef BMDBUSINTERFACE_H
#define BMDBUSINTERFACE_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface com.deepin.bootmaker
 */
class BMDBusInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    {
        return "com.deepin.bootmaker";
    }

public:
    BMDBusInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = nullptr);

    ~BMDBusInterface();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<QString> DeviceList()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("DeviceList"), argumentList);
    }

    inline QDBusPendingReply<bool> Install(const QString &image, const QString &device, const QString &partition, bool formatDevice)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(image) << QVariant::fromValue(device) << QVariant::fromValue(partition) << QVariant::fromValue(formatDevice);
        return asyncCallWithArgumentList(QStringLiteral("Install"), argumentList);
    }

    inline QDBusPendingReply<bool> CheckFile(const QString &filepath)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(filepath);
        return asyncCallWithArgumentList(QStringLiteral("CheckFile"), argumentList);
    }

    inline QDBusPendingReply<> Reboot()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Reboot"), argumentList);
    }

    inline QDBusPendingReply<> Start()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Start"), argumentList);
    }

    inline QDBusPendingReply<> Stop()
    {
        QList<QVariant> argumentList;
        return callWithArgumentList(QDBus::CallMode::Block, QStringLiteral("Stop"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void DeviceListChanged(const QString &addlistJson, const QString &dellistJson);
    void Finished(int errcode, const QString &description);
    void ReportProgress(int current, int total, const QString &title, const QString &description);
    void ReportProgress1(int current, int total, const QString &title, const QString &description);
    //    void CheckFileResult(bool result);
};

namespace com {
namespace deepin {
typedef ::BMDBusInterface bootmaker;
}
}
#endif
