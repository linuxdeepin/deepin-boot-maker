/**
 * Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#pragma once

#include <QObject>
#include <QDBusContext>
#include <QScopedPointer>

class BootMakerServicePrivate;
class BootMakerService :  public QObject,
        protected QDBusContext
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.bootmaker")

public:
    explicit BootMakerService(QObject *parent = 0);
    virtual ~BootMakerService();

signals:
    Q_SCRIPTABLE void DeviceListChanged(const QString deviceListJson);
    Q_SCRIPTABLE void Finished(int errcode, const QString &description);
    Q_SCRIPTABLE void ReportProgress(int current, int error, const QString &title, const QString &description);

public slots:
    Q_SCRIPTABLE void Reboot();
    Q_SCRIPTABLE void Start();
    Q_SCRIPTABLE void Stop();
    Q_SCRIPTABLE QString DeviceList();
    Q_SCRIPTABLE bool Install(const QString &image,
                 const QString &device,
                 const QString &partition,
                 bool  formatDevice);

signals:
    void startInstall(const QString &, const QString &, const QString &, bool);

private:
    QScopedPointer<BootMakerServicePrivate> d_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(d_ptr), BootMakerService)
};

