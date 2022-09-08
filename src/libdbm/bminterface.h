// SPDX-FileCopyrightText: 2016 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once
#include "util/deviceinfo.h"

#include <DSingleton>

#include <QObject>
#include <QScopedPointer>

class BMInterfacePrivate;
class BMInterface : public QObject, public Dtk::Core::DSingleton<BMInterface>
{
    Q_OBJECT
public:
    explicit BMInterface(QObject *parent = nullptr);
    ~BMInterface();

    static void initResources()
    {
#if defined(STATIC_LIB)
        Q_INIT_RESOURCE(blob);
#endif
    }

signals:
    void deviceListChanged(const QList<DeviceInfo> &addlist, const QList<DeviceInfo> &dellist);
    void finished(int errcode, const QString &description);
    void reportProgress(int current, int total, const QString &title, const QString &description);
    void reportProgress1(int current, int total, const QString &title, const QString &description);
    void startInstall(const QString &image,
                      const QString &device,
                      const QString &partition,
                      bool  formatDevice);
//    void checkFileResult(bool result);
public slots:
    void start();
    void stop();
    void reboot();
    QList<DeviceInfo> deviceList();
    bool checkfile(const QString &filepath);

private:
    bool install(const QString &image,
                 const QString &device,
                 const QString &partition,
                 bool  formatDevice);

    friend class Dtk::Core::DSingleton<BMInterface>;
    QScopedPointer<BMInterfacePrivate> d_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(d_ptr), BMInterface)
};

