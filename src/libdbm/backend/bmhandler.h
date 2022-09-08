// SPDX-FileCopyrightText: 2016 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once
#include "../util/deviceinfo.h"

#include <QObject>

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
        USBNotMountFailed,
        UMountUSBFailed,
        CheckImageIntegrityFailed,
        ExtractImgeFailed,
        InstallBootloaderFailed,
        GetUsbInstallDirFailed,
        SyncIOFailed,
        UnDefinedError
    };

    explicit BMHandler(QObject *parent = nullptr): QObject(parent)
    {
        connect(this, &BMHandler::startInstall,
                this, &BMHandler::install);
        connect(this, &BMHandler::startCheckfile,
                this, &BMHandler::checkfile);
    }

    virtual ~BMHandler() {

    }
    static const QString errorString(BMHandler::ErrorType et);
signals:
    void startInstall(const QString &, const QString &, const QString &, bool);
    void startCheckfile(const QString &filepath);
    void removablePartitionsChanged(const QList<DeviceInfo> &addlist, const QList<DeviceInfo>& reducelist);
    void finished(int errcode, const QString &description);
    void reportProgress(int current, int error, const QString &title, const QString &description);
   void  reportProgress1(int current, int error, const QString &title, const QString &description);
    //    void checkFileResult(bool result);

public slots:
    virtual void reboot() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual const QList<DeviceInfo> deviceList() const = 0;
    virtual bool checkfile(const QString &filepath) = 0;

protected:
    virtual bool install(const QString &image,
                         const QString &device,
                         const QString &partition,
                         bool  formatDevice) = 0;
};


