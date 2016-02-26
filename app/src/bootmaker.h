/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef USBCREATOR_H
#define USBCREATOR_H

#include <QString>
#include <QStringList>
#include <QWidget>

class unetbootin;
class FileListMonitor;
class ProcessRate;
class UsbInstaller;

class BootMaker : public QWidget {
    Q_OBJECT
public:
    explicit BootMaker(QWidget* parent = 0);

    enum ProcessStatus{
        Processing,
        Finish,
        Failed,
    };

    bool start(QString isoPath, QString usbDriver, bool formatDisk = false);

    ProcessStatus status();

    QString errmsg();
    int processRate();

    bool isISOImage(QString isoPath);
    void exitRestart();

    bool confirmFormatDlg();
    bool checkInstallPara();

public slots:
    void reboot();
    void error(const QString &errmsg);
    void finish();

signals:
    void process(const QString&, const QString&, bool);
    void listUsbDevice(const QStringList& devicelist);

    void start();

protected:
    QString             m_ImagePath;
    QString             m_DriverPath;
    bool                m_FormatDisk;
    FileListMonitor     *m_FileListMonitor;
    ProcessRate         *m_Progress;
    UsbInstaller        *m_Installer;

    ProcessStatus       m_status;
    QString             m_error;
};


const QString AppTitle();

#endif // USBCREATOR_H
