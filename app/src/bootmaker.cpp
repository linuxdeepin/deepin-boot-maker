/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "bootmaker.h"

#include <XSys>

#include "utils.h"
#include "usbinstaller.h"
#include "progressrate.h"

#include <QThread>
#include <QMessageBox>
#include <QFileInfo>
#include <QDebug>

#include <memory>

#ifdef Q_OS_WIN32
#include <windows.h>
#include <shellapi.h>
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "advapi32.lib")
#endif

#ifdef Q_OS_LINUX
#include <unistd.h>
#include <sys/reboot.h>
#endif

const QString AppTitle() {
    return QObject::tr("Deepin Boot Maker");
}

BootMaker::BootMaker(QWidget* parent) : QWidget(parent) {
    m_FileListMonitor = new FileListMonitor();
    m_Progress = new ProcessRate();
    m_Installer = new UsbInstaller();
    m_Installer->fileMonitor = m_FileListMonitor;
    m_Installer->progress = m_Progress;

    QThread *pwork = new QThread();
    m_Installer->moveToThread(pwork);
    connect(m_Installer, SIGNAL(listUsbDevice(QStringList)),
            this, SIGNAL(listUsbDevice(QStringList)));
    connect(this, SIGNAL(start()),
            m_Installer, SLOT(start()));
    pwork->start();
    this->start();
}

bool BootMaker::start(QString isoPath, QString usbDriver, bool formatDisk) {
    m_ImagePath = isoPath;
    m_DriverPath = usbDriver;
    m_FormatDisk = formatDisk;

    if(this->checkInstallPara()) {
        if((!formatDisk) && (!Utils::CheckInstallDisk(usbDriver))) {
            QMessageBox msgbox;
            msgbox.setIcon(QMessageBox::Critical);
            msgbox.setWindowTitle(tr("Format error of USB flash drive"));
            msgbox.setText(tr("Only FAT32 USB flash drive supported. Need to format? All partitions and data will be lost during formatting, please back up the data in advance."));
            msgbox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgbox.setButtonText(QMessageBox::Ok, tr("Format"));
            msgbox.setButtonText(QMessageBox::Cancel, tr("Cancel"));

            if(msgbox.exec() == QMessageBox::Ok) {
                m_FormatDisk = true;
            } else {
                return false;
            }
        }

        qDebug() << "Start make boot disk";

        QObject::connect(this, SIGNAL(process(QString, QString, bool)),
                         m_Installer, SLOT(installUSB(QString, QString, bool)));

        connect(this->m_Installer, SIGNAL(error(QString)),
                this, SLOT(error(QString)));
        connect(this->m_Installer, SIGNAL(finish()),
                this, SLOT(finish()));

        emit this->process(m_ImagePath, m_DriverPath, m_FormatDisk);
        m_status = Processing;
        qDebug() << "BootMaker Return";
        return true;
    }

    return false;
}


void BootMaker::finish() {
    m_status = Finish;
}

void BootMaker::error(const QString &errmsg) {
    m_status = Failed;
    m_error = errmsg;
}

QString BootMaker::errmsg() {
    return m_error;
}

int BootMaker::processRate() {
    if(!(m_Installer->isFinsh_)) {
        m_Progress->setValue(m_FileListMonitor->FinishSize());
    } else {
        m_Progress->setValue(m_Progress->maximum());
    }

    qDebug() << QString("processRate value: %1/total: %2, rate: %3").arg(m_Progress->value()).arg(m_Progress->maximum()).arg(m_Progress->rate());
    return m_Progress->rate();
}

BootMaker::ProcessStatus BootMaker::status() {
    qDebug()<<m_status;
    return m_status;
}

bool BootMaker::isISOImage(QString isoPath) {
    QFileInfo fileinfo(isoPath);

    if(fileinfo.suffix() == "iso")
        return true;

    QMessageBox msg;
    msg.setIcon(QMessageBox::Information);
    msg.setWindowTitle(tr("Please select an iso image"));
    msg.setText(tr("Please select an iso image"));
    msg.setStandardButtons(QMessageBox::Ok);
    msg.exec();
    return false;
}

bool BootMaker::confirmFormatDlg() {
    QMessageBox msgbox(this);
    msgbox.setIcon(QMessageBox::Warning);
    msgbox.setWindowTitle(tr("Format USB flash drive"));
    msgbox.setText(tr("All data will be lost during formatting, please back up in advance and then press OK button."));
    msgbox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgbox.setButtonText(QMessageBox::Ok, tr("Ok"));
    msgbox.setButtonText(QMessageBox::Cancel, tr("Cancel"));

    if(msgbox.exec() == QMessageBox::Ok) {
        return true;
    } else {
        return false;
    }
}

void BootMaker::reboot() {
#ifdef Q_OS_WIN32
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0);
    ExitWindowsEx(EWX_REBOOT, EWX_FORCE);
#endif
#ifdef Q_OS_LINUX
    sync();
    ::reboot(RB_AUTOBOOT);
#endif
#ifdef Q_OS_MAC
    XSys::SynExec("shutdown", "-r now &");
#endif
}

void BootMaker::exitRestart() {
    reboot();
}


bool BootMaker::checkInstallPara() {
    if(m_DriverPath.isEmpty()) {
        QMessageBox unotenoughinputmsgb(this);
        unotenoughinputmsgb.setIcon(QMessageBox::Information);
        unotenoughinputmsgb.setWindowTitle(tr("Insert a USB flash drive"));
        unotenoughinputmsgb.setText(tr("No USB flash drives were found. If you have already inserted a USB flash drive, try to reformat it as FAT32."));
        unotenoughinputmsgb.setStandardButtons(QMessageBox::Ok);
        unotenoughinputmsgb.setButtonText(QMessageBox::Ok, tr("Ok"));
        unotenoughinputmsgb.exec();
        return false;
    }

#ifdef Q_OS_MAC
    if(XSys::DiskUtil::MountPoint(m_DriverPath) == "")
        XSys::SynExec("diskutil", "mount " + m_DriverPath);
#endif

#ifdef Q_OS_LINUX
    // TODO: Mount it auto
    if(XSys::DiskUtil::MountPoint(m_DriverPath) == "") {
        QString mountPoint = XSys::FS::TmpFilePath("");
        bool ret = XSys::DiskUtil::Mount(m_DriverPath, mountPoint);
        if (!ret) {
            QMessageBox merrordevnotmountedmsgbx(this);
            merrordevnotmountedmsgbx.setIcon(QMessageBox::Warning);
            merrordevnotmountedmsgbx.setWindowTitle(QString(tr("%1 not mounted")).arg(m_DriverPath));
            merrordevnotmountedmsgbx.setText(QString(tr("You must firstly mount the USB flash drive %1 to a mountpoint. Most distributions will do this automatically after you remove and reinsert the USB flash drive.")).arg(m_DriverPath));
            merrordevnotmountedmsgbx.setStandardButtons(QMessageBox::Ok);
            merrordevnotmountedmsgbx.setButtonText(QMessageBox::Ok, tr("Ok"));
            merrordevnotmountedmsgbx.exec();
            return false;
        }
    }
#endif

    if(m_ImagePath.isEmpty()) {
        QMessageBox fnotenoughinputmsgb(this);
        fnotenoughinputmsgb.setIcon(QMessageBox::Information);
        fnotenoughinputmsgb.setWindowTitle(tr("Select a disk image file"));
        fnotenoughinputmsgb.setText(tr("You must select a disk image file to load."));
        fnotenoughinputmsgb.setStandardButtons(QMessageBox::Ok);
        fnotenoughinputmsgb.setButtonText(QMessageBox::Ok, tr("Ok"));
        fnotenoughinputmsgb.exec();
        return false;
    }

    if(!QFile::exists(m_ImagePath)) {
        QMessageBox ffnotexistsmsgb(this);
        ffnotexistsmsgb.setIcon(QMessageBox::Information);
        ffnotexistsmsgb.setWindowTitle(tr("Disk image file not found"));
        ffnotexistsmsgb.setText(tr("The specified disk image file %1 does not exist.").arg(m_ImagePath));
        ffnotexistsmsgb.setStandardButtons(QMessageBox::Ok);
        ffnotexistsmsgb.setButtonText(QMessageBox::Ok, tr("Ok"));
        ffnotexistsmsgb.exec();
        return false;
    }

    return true;
}
