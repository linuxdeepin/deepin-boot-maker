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

private:
    QScopedPointer<BootMakerServicePrivate> d_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(d_ptr), BootMakerService)
};

