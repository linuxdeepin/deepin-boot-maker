#include "qtinstallerfactory.h"

#ifdef Q_PROCESSOR_X86
#include "qtX86Installer.h"
#elif Q_PROCESSOR_MIPS
#include "qtmipsinstaller.h"
#elif Q_PROCESSOR_ARM
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

#ifdef Q_PROCESSOR_X86
    qDebug() << "Architecture:X86_64";
    pInstaller = new QtX86Installer;
#elif Q_PROCESSOR_MIPS
    qDebug() << "Architecture:MIPS";
    pInstaller = new QtMipsInstaller;
#elif Q_PROCESSOR_ARM
#error "not support architecture"
#endif

    return pInstaller;
}

QtInstallerFactory::QtInstallerFactory(QObject *parent) : QObject(parent)
{

}
