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
#include <QMap>
#include <QScopedPointer>

class BootMakerServicePrivate;
class BootMakerService : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.bootmaker")

public:
    explicit BootMakerService(QObject *parent = 0);
    ~BootMakerService();

signals:
    void DeviceListChanged(const QString deviceListJson);
    void Finished(int errcode, const QString &description);
    void ReportProgress(int current, int error, const QString &title, const QString &description);

public slots:
    QString DeviceList();
    bool Install(const QString &image,
                 const QString &device,
                 const QString &partition,
                 bool  formatDevice);

signals:
    void startInstall(const QString &, const QString &, const QString &, bool);

private:
    QScopedPointer<BootMakerServicePrivate> d_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(d_ptr), BootMakerService)
};

