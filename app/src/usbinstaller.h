/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef USBINSTALLER_H
#define USBINSTALLER_H

#include "progressrate.h"
#include <QObject>

class QTimer;
class FileListMonitor;

class UsbInstaller : public QObject {
    Q_OBJECT
public:
    explicit UsbInstaller(QObject *parent = 0);

    FileListMonitor *fileMonitor;
    ProcessRate *progress;
    QString installPath;
    bool isFinsh_;

signals:
    void listUsbDevice(const QStringList& devicelist);
    void error(const QString& errMsg);
    void finish();

public slots:
    void start();
    bool installUSB(const QString &isopath, const QString &devpath, bool format);
    bool extractISO(const QString &isopath, const QString installdir);
    void finishInstall();

public:
    bool isFinsh();

protected slots:
    void listUsbDrives();

protected:
    QTimer *m_refreshTimer;
};


#endif // USBINSTALLER_H
