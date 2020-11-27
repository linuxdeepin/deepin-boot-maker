#include "qtbaseinstaller.h"
#include "../util/utils.h"

#include <QFileInfo>
#include <QDebug>
#include <XSys>
#include <QVector>

QtBaseInstaller::QtBaseInstaller(QObject *parent) : QObject(parent)
  ,m_sevenZipCheck("","")
{

}

void QtBaseInstaller::setformat(bool bFormat)
{
    m_bFormat = bFormat;
}

void QtBaseInstaller::setPartionName(const QString& strPartionName)
{
    m_strPartionName = strPartionName;
}

void QtBaseInstaller::setImage(const QString& strImage)
{
    m_strImage = strImage;
}

void QtBaseInstaller::beginInstall()
{
    bool bRet = hasEnoughSpace();

    if (!bRet) {
        checkError();
        return;
    }
    else {
        emit this->reportProgress(5, "check usb space finished", "");
    }

    bRet = checkISOIntegrity();

    if (!bRet) {
        checkError();
        return;
    }
    else {
        emit this->reportProgress(10, "check integrity finished", "");
    }

    bRet = formatUsb();

    if (!bRet) {
        checkError();
        return;
    }
    else {
        emit this->reportProgress(15, "format usb finished", "");
    }

    bRet = installBootload();

    if (!bRet) {
        checkError();
        return;
    }
    else {
        emit this->reportProgress(20, "install bootloader finished", "");
    }

    bRet = extractISO();

    if (!bRet) {
        checkError();
        return;
    }
    else {
        emit this->reportProgress(80, "extract ISO finished", "");
    }

    emit this->reportProgress(81, "begin sync IO", "");
    bRet = syncIO();

    if (!bRet) {
        checkError();
        return;
    }
    else {
        emit this->reportProgress(90, "sync IO finished", "");
    }

    configSyslinux();
    emit this->reportProgress(100, "finish", "");

    bRet = ejectDisk();

    if (!bRet) {
        checkError();
        return;
    }
    else {
        emit this->reportProgress(101, "finish", "");
    }
}

void QtBaseInstaller::checkError()
{
    qDebug() << "begin check error";
    QString strDisk = XSys::DiskUtil::GetPartitionDisk(m_strPartionName);

    if (strDisk.isEmpty()) {
        qCritical() << "Error::get(Error::USBMountFailed)";
        emit progressfinished(m_progressStatus, BMHandler::ErrorType::USBMountFailed);
        return;
    }
    else {
        QStringList strPartions = XSys::DiskUtil::GetPartionOfDisk(strDisk);

        if (!strPartions.contains(m_strPartionName)) {
            qCritical() << "Error::get(Error::USBMountFailed)";
            emit progressfinished(m_progressStatus, BMHandler::ErrorType::USBMountFailed);
            return;
        }
    }

    if (m_progressStatus == CHECKSPACE) {
        qCritical() << "Error::get(Error::USBSizeError)";
        emit progressfinished(m_progressStatus, BMHandler::ErrorType::USBSizeError);
        return;
    }
    else if (m_progressStatus == CHECKINTEGRITY) {
        qCritical() << "Error::get(Error::CheckImageIntegrityFailed)";
        emit progressfinished(m_progressStatus, BMHandler::ErrorType::CheckImageIntegrityFailed);
        return;
    }
    else if (m_progressStatus == FORMATUSB) {
        qCritical() << "Error::get(Error::USBFormatError)";
        emit progressfinished(m_progressStatus, BMHandler::ErrorType::USBFormatError);
    }
    else if (m_progressStatus == INSTALLBOOTLOADER) {
        qCritical() << "Error::get(Error::InstallBootloaderFailed)";
        emit progressfinished(m_progressStatus, BMHandler::ErrorType::InstallBootloaderFailed);
    }
    else if (m_progressStatus == GETINSTALLDIR) {
        qCritical() << "Error::get(Error::GetUsbInstallDirFailed)";
        emit progressfinished(m_progressStatus, BMHandler::ErrorType::GetUsbInstallDirFailed);
    }
    else if (m_progressStatus == EXTRACTISO) {
        qCritical() << "Error::get(Error::ExtractImgeFailed)";
        emit progressfinished(m_progressStatus, BMHandler::ErrorType::ExtractImgeFailed);
    }
    else if (m_progressStatus == SYNCIO) {
        qCritical() << "Error::get(Error::SyncIOFailed)";
        emit progressfinished(m_progressStatus, BMHandler::ErrorType::SyncIOFailed);
    }
    else {
        qCritical() << "Error::get(Error::UnDefinedError)";
        emit progressfinished(m_progressStatus, BMHandler::ErrorType::UnDefinedError);
    }
}

QString QtBaseInstaller::getMountPoint()
{
    return  "";
}

bool QtBaseInstaller::ejectDisk()
{
    qDebug() << "begin eject disk";
    XSys::Result result = XSys::DiskUtil::EjectDisk(m_strPartionName);
    return result.isSuccess();
}

bool QtBaseInstaller::hasEnoughSpace()
{
    bool bRet = false;
    qDebug() << "begin check space";
    m_progressStatus = CHECKSPACE;
    QFileInfo isoInfo(m_strImage);

#define KByt 1024
    if (m_bFormat) {
        if (isoInfo.size() / KByt > XSys::DiskUtil::GetPartitionTotalSpace(m_strPartionName)) {
            qCritical() << "Error::get(Error::USBSizeError)";
            bRet = false;
        }
        else {
            bRet = true;
        }
    } else {
        if (isoInfo.size() / KByt > XSys::DiskUtil::GetPartitionFreeSpace(m_strPartionName)) {
            qCritical() << "Error::get(Error::USBSizeError)";
            bRet = false;
        }
        else {
            bRet = true;
        }
    }

    return bRet;
}

bool QtBaseInstaller::checkISOIntegrity()
{
    bool bRet = false;
    qDebug() << "check iso integrity.";
    m_progressStatus = CHECKINTEGRITY;

    //check iso integrity
    m_sevenZipCheck.setArchiveFile(m_strImage);
    m_sevenZipCheck.setOutputDirectory("");

    if (!m_sevenZipCheck.check()) {
        qCritical() << "Error::get(Error::ExtractImgeFailed)";
        bRet = false;
    }
    else {
        bRet = true;
    }

    return bRet;
}

bool QtBaseInstaller::formatUsb()
{
    qDebug() << "begin format usb.";
    m_progressStatus = FORMATUSB;

    if (!umountPartion()) {
        return false;
    }

    if (m_bFormat) {
        if (!XSys::DiskUtil::FormatPartion(m_strPartionName)) {
            return false;
        }
    }

    return true;
}

bool QtBaseInstaller::installBootload()
{
    qDebug() << "begin install bootloader on" << m_strPartionName;
    m_progressStatus = INSTALLBOOTLOADER;
    XSys::Result result = XSys::Bootloader::Syslinux::InstallSyslinux(m_strPartionName, m_strImage);
    return result.isSuccess();
}

bool QtBaseInstaller::extractISO()
{
    qDebug() << "begin extract ISO to" << m_strPartionName;
    m_progressStatus = GETINSTALLDIR;
    QString installDir = XSys::DiskUtil::MountPoint(m_strPartionName);

    if (installDir.isEmpty()) {
        qCritical() << "Error::get(Error::USBMountFailed)";
        return false;
    }

    qDebug() << "begin clear target device files";
    m_progressStatus = EXTRACTISO;
    Utils::ClearTargetDev(installDir);
    m_sevenZipCheck.setArchiveFile(m_strImage);
    m_sevenZipCheck.setOutputDirectory(installDir);
    return m_sevenZipCheck.extract();
}

bool QtBaseInstaller::syncIO()
{
    qDebug() << "begin sysc IO";
    return XSys::SynExec("sync", "").isSuccess();
}

bool QtBaseInstaller::configSyslinux()
{
    qDebug() << "begin configure syslinux";
    XSys::SynExec("sync", "");
    QString installDir = XSys::DiskUtil::MountPoint(m_strPartionName);
    qDebug() << "configure syslinux, installDir:" << installDir;
    return XSys::Bootloader::Syslinux::ConfigSyslinx(installDir).isSuccess();
}
