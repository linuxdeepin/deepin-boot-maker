// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "qtinstallerfactory.h"

#if defined(Q_PROCESSOR_X86)
#include "qtX86Installer.h"
#elif defined(Q_PROCESSOR_MIPS)
#include "qtmipsinstaller.h"
#elif defined(Q_PROCESSOR_ARM)
#include "qtarminstaller.h"
#elif defined(__sw_64__)
#include "qtswinstaller.h"
#else
#include "qtotherinstaller.h"
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
    pInstaller = new QtX86Installer();
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
    qDebug() << "Architecture:Other";
    pInstaller = new QtOtherInstaller;
#endif

    return pInstaller;
}

QtInstallerFactory::QtInstallerFactory(QObject *parent) : QObject(parent)
{

}
