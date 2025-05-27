// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "qtX86Installer.h"

#include <XSys>
#include <QDebug>

QtX86Installer::QtX86Installer(QObject *parent) : QtBaseInstaller (parent)
{
    qDebug() << "Initializing X86 installer";
}

bool QtX86Installer::installBootload()
{
    qDebug() << "Starting bootloader installation on partition:" << m_strPartionName;
    m_progressStatus = INSTALLBOOTLOADER;

    QString strDisk = XSys::DiskUtil::GetPartitionDisk(m_strPartionName);

    if (strDisk.isEmpty()) {
        qCritical() << "Failed to get partition disk information";
        return false;
    }

    qDebug() << "Installing bootloader on partition:" << m_strPartionName;
    XSys::Result result = XSys::Syslinux::InstallBootloader(m_strPartionName);

    if (!result.isSuccess()) {
        qCritical() << "Failed to install bootloader:" << result.errmsg();
        return false;
    }

    qDebug() << "Installing MBR on disk:" << strDisk;
    result = XSys::Syslinux::InstallMbr(strDisk);

    if (!result.isSuccess()) {
        qCritical() << "Failed to install MBR:" << result.errmsg();
        return false;
    }

    qDebug() << "Setting active partition:" << m_strPartionName << "on disk:" << strDisk;
    if (!XSys::DiskUtil::SetActivePartion(strDisk, m_strPartionName)) {
        qCritical() << "Failed to set active partition";
        return false;
    }

    qDebug() << "Setting partition label for image:" << m_strImage;
    XSys::DiskUtil::SetPartionLabel(m_strPartionName, m_strImage);
    
    qInfo() << "Bootloader installation completed successfully";
    return true;
}
