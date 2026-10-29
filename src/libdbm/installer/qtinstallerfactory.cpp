// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "../util/ddlog.h"
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
        qCDebug(installerLog) << "Creating new QtInstallerFactory instance";
        m_pSelf = new QtInstallerFactory;
    }

    return m_pSelf;
}

QtBaseInstaller* QtInstallerFactory::createInstaller()
{
    qCDebug(installerLog) << "Creating platform-specific installer";
    QtBaseInstaller* pInstaller = nullptr;

#if defined(Q_PROCESSOR_X86)
    qCDebug(installerLog) << "Detected X86_64 architecture, creating X86 installer";
    pInstaller = new QtX86Installer();
#elif defined(Q_PROCESSOR_MIPS)
    qCDebug(installerLog) << "Detected MIPS architecture, creating MIPS installer";
    pInstaller = new QtMipsInstaller;
#elif defined(Q_PROCESSOR_ARM)
    qCDebug(installerLog) << "Detected ARM architecture, creating ARM installer";
    pInstaller = new QtArmInstaller;
#elif defined(__sw_64__)
    qCDebug(installerLog) << "Detected SW architecture, creating SW installer";
    pInstaller = new QtSwInstaller;
#else
    qCDebug(installerLog) << "Detected unknown architecture, creating generic installer";
    pInstaller = new QtOtherInstaller;
#endif

    return pInstaller;
}

QtInstallerFactory::QtInstallerFactory(QObject *parent) : QObject(parent)
{
    qCDebug(installerLog) << "Initializing QtInstallerFactory";
}
