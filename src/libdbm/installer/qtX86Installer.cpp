// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "qtX86Installer.h"

#include <XSys>
#include <QDebug>

QtX86Installer::QtX86Installer(QObject *parent) : QtBaseInstaller (parent)
{

}

bool QtX86Installer::installBootload()
{
    qDebug() << "begin install bootloader on" << m_strPartionName;
    m_progressStatus = INSTALLBOOTLOADER;

    QString strDisk = XSys::DiskUtil::GetPartitionDisk(m_strPartionName);

    if (strDisk.isEmpty()) {
        return false;
    }

    XSys::Result result = XSys::Syslinux::InstallBootloader(m_strPartionName);

    if (!result.isSuccess()) {
        return false;
    }

    result = XSys::Syslinux::InstallMbr(strDisk);

    if (!result.isSuccess()) {
        return false;
    }

    if (!XSys::DiskUtil::SetActivePartion(strDisk, m_strPartionName)) {
        return false;
    }

    XSys::DiskUtil::SetPartionLabel(m_strPartionName, m_strImage);
    return true;
}
