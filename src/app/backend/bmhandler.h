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
#include "../util/deviceinfo.h"

class BMHandler : public QObject
{
    Q_OBJECT
public:
    enum ErrorType {
        NoError = 0,
        SyscExecFailed,
        USBFormatError,
        USBSizeError,
        USBMountFailed,
        ExtractImgeFailed,
    };

    explicit BMHandler(QObject *parent = 0): QObject(parent) {}

    static const QString errorString(ErrorType et);
signals:
    void removablePartitionsChanged(const QList<DeviceInfo> &list);
    void finished(int errcode, const QString &description);
    void reportProgress(int current, int error, const QString &title, const QString &description);

public slots:
    virtual void reboot() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual const QList<DeviceInfo> deviceList() const = 0;
    virtual bool install(const QString &image,
                         const QString &device,
                         const QString &partition,
                         bool  formatDevice) = 0;
};


