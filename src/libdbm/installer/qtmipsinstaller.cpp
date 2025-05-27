// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "qtmipsinstaller.h"

#include <XSys>
#include <QDebug>

QtMipsInstaller::QtMipsInstaller(QObject *parent) : QtBaseInstaller (parent)
{
    qDebug() << "Initializing MIPS installer";
}

bool QtMipsInstaller::installBootload()
{
    qInfo() << "Starting bootloader installation on partition:" << m_strPartionName;
    m_progressStatus = INSTALLBOOTLOADER;

    QString strDisk = XSys::DiskUtil::GetPartitionDisk(m_strPartionName);

    if (strDisk.isEmpty()) {
        qCritical() << "Failed to get partition disk information";
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
