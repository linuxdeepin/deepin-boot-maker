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

#include "qtmipsinstaller.h"

#include <XSys>
#include <QDebug>

QtMipsInstaller::QtMipsInstaller(QObject *parent) : QtBaseInstaller (parent)
{

}

bool QtMipsInstaller::installBootload()
{
    qInfo() << "begin install bootloader on" << m_strPartionName;
    m_progressStatus = INSTALLBOOTLOADER;

    QString strDisk = XSys::DiskUtil::GetPartitionDisk(m_strPartionName);

    if (strDisk.isEmpty()) {
        return false;
    }

    if (!XSys::DiskUtil::SetActivePartion(strDisk, m_strPartionName)) {
        return false;
    }

    XSys::DiskUtil::SetPartionLabel(m_strPartionName, m_strImage);
    return true;
}
