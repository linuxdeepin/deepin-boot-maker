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

#include "qtinstallerfactory.h"

#if defined(Q_PROCESSOR_X86)
#include "qtX86Installer.h"
#elif defined(Q_PROCESSOR_MIPS)
#include "qtmipsinstaller.h"
#elif defined(Q_PROCESSOR_ARM)
#include "qtarminstaller.h"
#elif defined(__sw_64__)
#include "qtswinstaller.h"
#endif

#include <QDebug>

QtInstallerFactory*  QtInstallerFactory::m_pSelf = nullptr;

QtInstallerFactory* QtInstallerFactory::getInstance()
{
    if (nullptr == m_pSelf) {
        m_pSelf = new QtInstallerFactory;
    }

    return m_pSelf;
}

QtBaseInstaller* QtInstallerFactory::createInstaller()
{
    QtBaseInstaller* pInstaller = nullptr;

#if defined(Q_PROCESSOR_X86)
    qDebug() << "Architecture:X86_64";
    pInstaller = new QtX86Installer;
#elif defined(Q_PROCESSOR_MIPS)
    qDebug() << "Architecture:MIPS";
    pInstaller = new QtMipsInstaller;
#elif defined(Q_PROCESSOR_ARM)
    qDebug() << "Architecture:ARM";
    pInstaller = new QtArmInstaller;
#elif defined(__sw_64__)
    qDebug() << "Architecture:SW";
    pInstaller = new QtSwInstaller;
#else
#error "not support architecture"
#endif

    return pInstaller;
}

QtInstallerFactory::QtInstallerFactory(QObject *parent) : QObject(parent)
{

}
