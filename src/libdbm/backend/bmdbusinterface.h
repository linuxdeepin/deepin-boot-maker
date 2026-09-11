// SPDX-FileCopyrightText: 2016 - 2026 UnionTech Software Technology Co., Ltd.
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

#include <fcntl.h>
#include <errno.h>
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
        // The service runs with ProtectHome=true and cannot see /home/...
        // paths. Open the ISO here (in the caller's process, which can) and
        // pass only the descriptor over D-Bus; the service reopens it via
        // /proc/self/fd/<n>. The path itself never crosses the bus.
        QDBusUnixFileDescriptor fd = openReadOnlyFd(image);
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(device) << QVariant::fromValue(partition) << QVariant::fromValue(formatDevice) << QVariant::fromValue(fd);
        return asyncCallWithArgumentList(QStringLiteral("Install"), argumentList);
    }

    inline QDBusPendingReply<bool> CheckFile(const QString &filepath)
    {
        QDBusUnixFileDescriptor fd = openReadOnlyFd(filepath);
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(fd);
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

private:
    // The service runs with ProtectHome=true and cannot reach /home by path;
    // passing the descriptor lets it reopen the file via /proc/self/fd/<n>.
    // Returns an invalid descriptor on failure.
    // Opens the file read-only and hands the descriptor to the wrapper with
    // giveFileDescriptor(), which takes ownership: the descriptor is closed
    // when the wrapper is destroyed after the D-Bus message is sent. A plain
    // constructor call would dup the descriptor and leak the original.
    static QDBusUnixFileDescriptor openReadOnlyFd(const QString &path)
    {
        if (path.isEmpty()) {
            return QDBusUnixFileDescriptor();
        }
        int fd = ::open(path.toLocal8Bit().constData(), O_RDONLY | O_CLOEXEC);
        if (fd < 0) {
            return QDBusUnixFileDescriptor();
        }
        QDBusUnixFileDescriptor wrapper;
        wrapper.giveFileDescriptor(fd);
        return wrapper;
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
