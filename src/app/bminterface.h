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
#include <QScopedPointer>
#include <dsingleton.h>
#include "util/deviceinfo.h"

class BMInterfacePrivate;
class BMInterface : public QObject, public Dtk::DSingleton<BMInterface>
{
    Q_OBJECT
public:
    explicit BMInterface(QObject *parent = 0);
    ~BMInterface();

signals:
    void deviceListChanged(const QList<DeviceInfo> &partitions);
    void finished(int errcode, const QString &description);
    void reportProgress(int current, int total, const QString &title, const QString &description);

public slots:
    void start();
    void stop();
    QList<DeviceInfo> deviceList();
    bool install(const QString &image,
                 const QString &device,
                 const QString &partition,
                 bool  formatDevice);

private:
    friend class Dtk::DSingleton<BMInterface>;
    QScopedPointer<BMInterfacePrivate> d_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(d_ptr), BMInterface)
};

