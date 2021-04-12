/*
 * Copyright (C) 2020 ~ 2021 Uniontech Software Co., Ltd.
 *
 * Author:     shenfusheng <shenfusheng@uniontech.com>
 *
 * Maintainer: shenfusheng <shenfusheng@uniontech.com>
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
