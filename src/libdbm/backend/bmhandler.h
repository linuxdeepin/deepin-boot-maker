/*
 * Copyright (C) 2016 ~ 2018 Wuhan Deepin Technology Co., Ltd.
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

    explicit BMHandler(QObject *parent = 0): QObject(parent) {
        connect(this, &BMHandler::startInstall,
                this, &BMHandler::install);
    }

    static const QString errorString(ErrorType et);
signals:
    void startInstall(const QString &, const QString &, const QString &, bool);
    void removablePartitionsChanged(const QList<DeviceInfo> &list);
    void finished(int errcode, const QString &description);
    void reportProgress(int current, int error, const QString &title, const QString &description);

public slots:
    virtual void reboot() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual const QList<DeviceInfo> deviceList() const = 0;

protected:
    virtual bool install(const QString &image,
                         const QString &device,
                         const QString &partition,
                         bool  formatDevice) = 0;
};


