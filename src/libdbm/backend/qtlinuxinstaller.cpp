#include "qtlinuxinstaller.h"

#include <XSys>
#include <QDebug>

QtLinuxInstaller::QtLinuxInstaller(QObject *parent) : QtBaseInstaller (parent)
{

}

bool QtLinuxInstaller::umountPartion()
{
    return XSys::DiskUtil::UmountPartion(m_strPartionName);
}

bool QtLinuxInstaller::umountDisk()
{
    bool bRet = false;
    QString device = XSys::DiskUtil::GetPartitionDisk(m_strPartionName);

    if (!XSys::DiskUtil::UmountDisk(device)) {
        qCritical() << "umount partion failed: ";
        bRet = false;
    }
    else {
        bRet = true;
    }

    return bRet;
}

QString QtLinuxInstaller::getMountPoint()
{
    return  XSys::DiskUtil::MountPoint(m_strPartionName);
}

bool QtLinuxInstaller::ejectDisk()
{
    qDebug() << "begin eject disk";
    m_progressStatus = EJECTDISK;

    if (!(umountDisk())) {
        return false;
    }

    return QtBaseInstaller::ejectDisk();
}
