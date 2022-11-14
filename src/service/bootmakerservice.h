// SPDX-FileCopyrightText: 2016 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

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
    explicit BootMakerService(QObject *parent = nullptr);
    virtual ~BootMakerService();

signals:
    Q_SCRIPTABLE void DeviceListChanged(const QString addlistJson, const QString& dellistJson);
    Q_SCRIPTABLE void Finished(int errcode, const QString &description);
    Q_SCRIPTABLE void ReportProgress(int current, int error, const QString &title, const QString &description);
    Q_SCRIPTABLE void ReportProgress1(int current, int error, const QString &title, const QString &description);
    //    Q_SCRIPTABLE void CheckFileResult(bool result);
    void s_StartBootMarker();

public slots:
    Q_SCRIPTABLE void Reboot();
    Q_SCRIPTABLE void Start();
    Q_SCRIPTABLE void Stop();
    Q_SCRIPTABLE QString DeviceList();
    Q_SCRIPTABLE bool Install(const QString &image,
                              const QString &device,
                              const QString &partition,
                              bool  formatDevice);
    Q_SCRIPTABLE bool CheckFile(const QString &filepath);

private:
    QScopedPointer<BootMakerServicePrivate> d_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(d_ptr), BootMakerService)
};

