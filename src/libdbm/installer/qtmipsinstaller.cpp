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
