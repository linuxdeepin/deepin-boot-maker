// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "bmhandler.h"
#include "../installer/qtbaseinstaller.h"
#include "../util/devicemonitor.h"
#include <istream>

class DeviceMonitor;
class BootMaker : public BMHandler
{
    Q_OBJECT
public:
    explicit BootMaker(QObject *parent = nullptr);
    virtual ~BootMaker() {

        if (m_pInstaller != nullptr) {
            delete m_pInstaller;
            m_pInstaller = nullptr;
        }

        if (m_monitorWork != nullptr) {
            m_monitorWork->quit();
            m_monitorWork->wait();
            m_monitorWork->deleteLater();
        }

    }
public slots:
    void reboot();
    void start();
    void stop();
    const QList<DeviceInfo> deviceList() const;
    bool install(const QString &image,
                 const QString &device,
                 const QString &partition,
                 bool  formatDevice);
    bool checkfile(const QString &filepath);

private:
    DeviceMonitor* m_usbDeviceMonitor = nullptr;
    QtBaseInstaller* m_pInstaller;
    QThread* m_monitorWork;
};

