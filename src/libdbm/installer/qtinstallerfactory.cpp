#include "qtinstallerfactory.h"

#if defined(Q_PROCESSOR_X86)
#include "qtX86Installer.h"
#elif defined(Q_PROCESSOR_MIPS)
#include "qtmipsinstaller.h"
#elif definded(Q_PROCESSOR_ARM)
#include "qtarminstaller.h"
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
#elif definded(Q_PROCESSOR_ARM)
    qDebug() << "Architecture:ARM";
    pInstaller = new QtArmInstaller;
#else
#error "not support architecture"
#endif

    return pInstaller;
}

QtInstallerFactory::QtInstallerFactory(QObject *parent) : QObject(parent)
{

}
