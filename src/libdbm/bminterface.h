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
#include <QScopedPointer>
#include <DSingleton>
#include "util/deviceinfo.h"

class BMInterfacePrivate;
class BMInterface : public QObject, public Dtk::Core::DSingleton<BMInterface>
{
    Q_OBJECT
public:
    explicit BMInterface(QObject *parent = 0);
    ~BMInterface();

    static void initResources()
    {
#if defined(STATIC_LIB)
        Q_INIT_RESOURCE(blob);
#endif
    }

signals:
    void deviceListChanged(const QList<DeviceInfo> &partitions);
    void finished(int errcode, const QString &description);
    void reportProgress(int current, int total, const QString &title, const QString &description);
    void startInstall(const QString &image,
                      const QString &device,
                      const QString &partition,
                      bool  formatDevice);
public slots:
    void start();
    void stop();
    void reboot();
    QList<DeviceInfo> deviceList();

private:
    bool install(const QString &image,
                 const QString &device,
                 const QString &partition,
                 bool  formatDevice);

    friend class Dtk::Core::DSingleton<BMInterface>;
    QScopedPointer<BMInterfacePrivate> d_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(d_ptr), BMInterface)
};

