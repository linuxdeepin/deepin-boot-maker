// SPDX-FileCopyrightText: 2020 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "../util/ddlog.h"
#include "qtX86Installer.h"

#include <XSys>
#include <QDebug>

QtX86Installer::QtX86Installer(QObject *parent) : QtBaseInstaller (parent)
{
    qCDebug(installerLog) << "Initializing X86 installer";
}

bool QtX86Installer::installBootload()
{
    qCDebug(installerLog) << "Starting bootloader installation on partition:" << m_strPartionName;
    m_progressStatus = INSTALLBOOTLOADER;

    QString strDisk = XSys::DiskUtil::GetPartitionDisk(m_strPartionName);

    if (strDisk.isEmpty()) {
        qCCritical(installerLog) << "Failed to get partition disk information";
        return false;
    }

    qCDebug(installerLog) << "Installing bootloader on partition:" << m_strPartionName;
    XSys::Result result = XSys::Syslinux::InstallBootloader(m_strPartionName);

    if (!result.isSuccess()) {
        qCCritical(installerLog) << "Failed to install bootloader:" << result.errmsg();
        return false;
    }

    qCDebug(installerLog) << "Installing MBR on disk:" << strDisk;
    result = XSys::Syslinux::InstallMbr(strDisk);

    if (!result.isSuccess()) {
        qCCritical(installerLog) << "Failed to install MBR:" << result.errmsg();
        return false;
    }

    qCDebug(installerLog) << "Setting active partition:" << m_strPartionName << "on disk:" << strDisk;
    if (!XSys::DiskUtil::SetActivePartion(strDisk, m_strPartionName)) {
        qCCritical(installerLog) << "Failed to set active partition";
        return false;
    }

    qCDebug(installerLog) << "Setting partition label for image:" << m_strImage;
    if (!XSys::DiskUtil::SetPartionLabel(m_strPartionName, m_strImage)) {
        qCCritical(installerLog) << "Failed to set partition label";
        return false;
    }
    
    qCInfo(installerLog) << "Bootloader installation completed successfully";
    return true;
}
