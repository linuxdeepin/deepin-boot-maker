// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "qtbaseinstaller.h"
#include "../util/utils.h"

#include <QFileInfo>
#include <QDebug>
#include <XSys>
#include <QVector>
#include <QDir>

QtBaseInstaller::QtBaseInstaller(QObject *parent) : QObject(parent)
  ,m_sevenZipCheck("","")
  ,m_bRunning(false)
  ,m_bStop(false)
{
    qDebug() << "Initializing QtBaseInstaller";
}

void QtBaseInstaller::setformat(bool bFormat)
{
    qDebug() << "Setting format flag:" << bFormat;
    m_bFormat = bFormat;
}

void QtBaseInstaller::setPartionName(const QString& strPartionName)
{
    qDebug() << "Setting partition name:" << strPartionName;
    m_strPartionName = strPartionName;
}

void QtBaseInstaller::setImage(const QString& strImage)
{
    qDebug() << "Setting image path:" << strImage;
    m_strImage = strImage;
}

void QtBaseInstaller::beginInstall()
{
    qDebug() << "Starting installation process";
    m_bRunning = true;
    m_bStop = false;
    bool bRet = hasEnoughSpace();

    if (!bRet) {
        qWarning() << "Insufficient space check failed";
        checkError();
        return;
    }
    else {
        qDebug() << "USB space check completed successfully";
        emit this->reportProgress(5, "check usb space finished", "");
    }

    if (m_bStop) {
        qDebug() << "Installation stopped by user";
        m_bRunning = false;
        return;
    }

    bRet = checkISOIntegrity();

    if (!bRet) {
        qWarning() << "ISO integrity check failed";
        checkError();
        return;
    }
    else {
        qDebug() << "ISO integrity check completed successfully";
        emit this->reportProgress(10, "check integrity finished", "");
    }

    if (m_bStop) {
        qDebug() << "Installation stopped by user";
        m_bRunning = false;
        return;
    }

    bRet = formatUsb();

    if (!bRet) {
        qWarning() << "USB formatting failed";
        checkError();
        return;
    }
    else {
        qDebug() << "USB formatting completed successfully";
        emit this->reportProgress(15, "format usb finished", "");
    }

    if (m_bStop) {
        qDebug() << "Installation stopped by user";
        m_bRunning = false;
        return;
    }

    bRet = installBootload();

    if (!bRet) {
        qWarning() << "Bootloader installation failed";
        checkError();
        return;
    }
    else {
        qDebug() << "Bootloader installation completed successfully";
        emit this->reportProgress(20, "install bootloader finished", "");
    }

    if (m_bStop) {
        qDebug() << "Installation stopped by user";
        m_bRunning = false;
        return;
    }

    bRet = extractISO();

    if (!bRet) {
        qWarning() << "ISO extraction failed";
        checkError();
        return;
    }
    else {
        qDebug() << "ISO extraction completed successfully";
        emit this->reportProgress(80, "extract ISO finished", "");
    }

    qDebug() << "Starting IO synchronization";
    emit this->reportProgress(81, "begin sync IO", "");

    if (m_bStop) {
        qInfo() << "Installation stopped by user";
        m_bRunning = false;
        return;
    }

    bRet = syncIO();

    if (!bRet) {
        qWarning() << "IO synchronization failed";
        checkError();
        return;
    }
    else {
        qDebug() << "IO synchronization completed successfully";
        emit this->reportProgress(90, "sync IO finished", "");
    }

    if (m_bStop) {
        qDebug() << "Installation stopped by user";
        m_bRunning = false;
        return;
    }

    qInfo() << "Configuring syslinux";
    configSyslinux();
    //通过判断镜像中是否存在anaconda的文件夹来推测该镜像的安装器

    if(needAddRepo()) {
        qInfo() << "Repository configuration needed - modifying boot files";
        modifyBootGrubFile("/EFI/BOOT/grub.cfg");
        modifyBootGrubFile("/syslinux/syslinux.cfg");
    } else {
        qInfo() << "No repository configuration needed";
    }

    qInfo() << "Installation process completed";
    emit this->reportProgress(100, "finish", "");

    if (m_bStop) {
        qInfo() << "Installation stopped by user";
        m_bRunning = false;
        return;
    }

    bRet = ejectDisk();

    if (!bRet) {
        qWarning() << "Disk ejection failed";
        checkError();
        return;
    }
    else {
        qInfo() << "Disk ejected successfully";
        emit this->reportProgress(101, "finish", "");
    }

    qInfo() << "Installation process finished";
    m_bRunning = false;
}

void QtBaseInstaller::checkError()
{
    qInfo() << "Checking installation error";
    m_bRunning = false;

    if (m_bStop) {
        qInfo() << "Installation stopped by user";
        return;
    }

    qInfo() << "Beginning error check";
    QString strDisk = XSys::DiskUtil::GetPartitionDisk(m_strPartionName);

    if (strDisk.isEmpty()) {
        qCritical() << "Failed to get partition disk information";
        emit progressfinished(m_progressStatus, BMHandler::ErrorType::USBMountFailed);
        return;
    }
    else {
        int iIndex = strDisk.lastIndexOf("/");
        QString strTempName;

        if (iIndex >= 0) {
            strDisk = strDisk.right(strDisk.length() - iIndex - 1);
        }

        bool bFind = false;
        QStringList strPartions = XSys::DiskUtil::GetPartionOfDisk(strDisk);

        if (!strPartions.contains(m_strPartionName)) {
            foreach (QString strName, strPartions) {
                QString strTemp = QString("/dev") + "/" + strName;

                if (strTemp == m_strPartionName) {
                    bFind = true;
                    break;
                }
            }
        }

        if (!bFind) {
            qCritical() << "Partition not found in disk partitions";
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

bool QtBaseInstaller::isRunning() const
{
    return m_bRunning;
}

void QtBaseInstaller::stopInstall()
{
    qInfo() << "Stopping installation process - Current status:" << m_progressStatus;
    m_bStop = true;

    if(EXTRACTISO == m_progressStatus || CHECKINTEGRITY == m_progressStatus) {
        qInfo() << "Stopping SevenZip process";
        m_sevenZipCheck.stopProcess();
    }
}

bool QtBaseInstaller::hasEnoughSpace()
{
    qInfo() << "Checking available space";
    m_progressStatus = CHECKSPACE;
    QFileInfo isoInfo(m_strImage);
    //分区没有挂载时是获取不到正确的可用空间和分区大小的,因些需要先进行挂载。
    QString strMountPt = XSys::DiskUtil::MountPoint(m_strPartionName);

    if (strMountPt.isEmpty()) {
        qInfo() << "Mounting partition for space check";
        XSys::DiskUtil::Mount(m_strPartionName);
    }

    strMountPt = XSys::DiskUtil::MountPoint(m_strPartionName);

    if (strMountPt.isEmpty()) {
        qCritical() << "Failed to get mount point for space check";
        return false;
    }

#define KByt 1024
    if (m_bFormat) {
        if (isoInfo.size() / KByt > XSys::DiskUtil::GetPartitionTotalSpace(m_strPartionName)) {
            qCritical() << "Insufficient total space on partition";
            return false;
        }
        else {
            qInfo() << "Sufficient total space available";
            return true;
        }
    } else {
        if (isoInfo.size() / KByt > XSys::DiskUtil::GetPartitionFreeSpace(m_strPartionName)) {
            qCritical() << "Insufficient free space on partition";
            return false;
        }
        else {
            qInfo() << "Sufficient free space available";
            return true;
        }
    }
}

bool QtBaseInstaller::checkISOIntegrity()
{
    qInfo() << "Checking ISO integrity";
    m_progressStatus = CHECKINTEGRITY;

    m_sevenZipCheck.setArchiveFile(m_strImage);
    m_sevenZipCheck.setOutputDirectory("");

    if (!m_sevenZipCheck.check()) {
        qCritical() << "ISO integrity check failed";
        return false;
    }
    else {
        qInfo() << "ISO integrity check passed";
        return true;
    }
}

bool QtBaseInstaller::umountPartion()
{
    qInfo() << "Unmounting partition:" << m_strPartionName;
    return XSys::DiskUtil::UmountPartion(m_strPartionName);
}

bool QtBaseInstaller::umountDisk()
{
    qInfo() << "Unmounting disk";
    bool bRet = false;
    QString device = XSys::DiskUtil::GetPartitionDisk(m_strPartionName);

    if (!XSys::DiskUtil::UmountDisk(device)) {
        qCritical() << "Failed to unmount disk:" << device;
        bRet = false;
    }
    else {
        qInfo() << "Disk unmounted successfully";
        bRet = true;
    }

    return bRet;
}

QString QtBaseInstaller::getMountPoint()
{
    return XSys::DiskUtil::MountPoint(m_strPartionName);
}

bool QtBaseInstaller::ejectDisk()
{
    qInfo() << "Ejecting disk";
    m_progressStatus = EJECTDISK;

    if (!(umountDisk())) {
        qCritical() << "Failed to unmount disk before ejection";
        return false;
    }

    QString strDisk = XSys::DiskUtil::GetPartitionDisk(m_strPartionName);
    XSys::Result result = XSys::DiskUtil::EjectDisk(strDisk);
    return result.isSuccess();
}

bool QtBaseInstaller::formatUsb()
{
    qInfo() << "Formatting USB device";
    m_progressStatus = FORMATUSB;

    if (!umountPartion()) {
        qCritical() << "Failed to unmount partition before formatting";
        return false;
    }

    if (m_bFormat) {
        if (!XSys::DiskUtil::FormatPartion(m_strPartionName)) {
            qCritical() << "Failed to format partition";
            return false;
        }
        qInfo() << "Partition formatted successfully";
    }

    return true;
}

bool QtBaseInstaller::installBootload()
{
    qInfo() << "Installing bootloader";
    return false;
}

bool QtBaseInstaller::extractISO()
{
    qInfo() << "Extracting ISO to partition:" << m_strPartionName;
    XSys::SynExec("partprobe", m_strPartionName);
    m_progressStatus = GETINSTALLDIR;
    //由于前面的命令中会自动挂载系统，而导致如果操作过快会获取挂载点为空，然后在后面再次进行挂载时又挂载失败。因此加一个延时，让系统内核状态同步完成。
    int iTestCount = 3;
    QString installDir;

    do {
        QThread::msleep(2000);
        installDir = XSys::DiskUtil::MountPoint(m_strPartionName);

        if (!installDir.isEmpty()) {
            break;
        }

        iTestCount--;
    } while(iTestCount > 0);

    if (installDir.isEmpty()) {
        qInfo() << "Mounting partition for extraction";
        XSys::DiskUtil::Mount(m_strPartionName);
        installDir = XSys::DiskUtil::MountPoint(m_strPartionName);
    }

    if (installDir.isEmpty()) {
        qCritical() << "Failed to get mount point for extraction";
        return false;
    }

    if (m_bStop) {
        qInfo() << "Extraction stopped by user";
        return true;
    }

    qInfo() << "Clearing target device files";
    m_progressStatus = EXTRACTISO;
    Utils::ClearTargetDev(installDir);
    m_sevenZipCheck.setArchiveFile(m_strImage);
    m_sevenZipCheck.setOutputDirectory(installDir);
    return m_sevenZipCheck.extract();
}

bool QtBaseInstaller::syncIO()
{
    qInfo() << "Synchronizing IO";
    m_progressStatus = SYNCIO;
    return XSys::SynExec("sync", "").isSuccess();
}

bool QtBaseInstaller::configSyslinux()
{
    qInfo() << "Configuring syslinux";
    XSys::SynExec("sync", "");
    QString installDir = XSys::DiskUtil::MountPoint(m_strPartionName);
    qDebug() << "Configuring syslinux in directory:" << installDir;
    bool result = XSys::Syslinux::ConfigSyslinx(installDir).isSuccess();
    if (result) {
        qInfo() << "Syslinux configuration completed successfully";
    } else {
        qCritical() << "Syslinux configuration failed";
    }
    return result;
}

bool QtBaseInstaller::needAddRepo()
{
    qInfo() << "Checking if repository configuration is needed";
    QStringList args;
    args << "-f" << "-i" << m_strImage;
    XSys::Result ret = XSys::SynExec("isoinfo", args);

    if (!ret.isSuccess()) {
        qWarning() << "Failed to execute isoinfo:" << ret.result();
        return false;
    }

    QStringList strList = ret.result().split("\n");
//    欧拉版判断方法
    QStringList strPackagesList = strList.filter("/Packages/anaconda_", Qt::CaseInsensitive);
    QStringList strReleaseList = strList.filter("/Packages/UnionTech_release_", Qt::CaseInsensitive);
//    行业版判断方法
    QStringList strAppStreamPackagesList = strList.filter("/AppStream/Packages/anaconda", Qt::CaseInsensitive);

    return ((!strPackagesList.isEmpty()&&!strReleaseList.isEmpty()) || !strAppStreamPackagesList.isEmpty());
}

void QtBaseInstaller::modifyBootGrubFile(QString grub_file_name)
{
    qInfo() << "Modifying boot grub file:" << grub_file_name;
    QString strMountPt = XSys::DiskUtil::MountPoint(m_strPartionName);
    QString strFullFileName = strMountPt + grub_file_name;

    if (QFile::exists(strFullFileName)) {
        QFile readFile(strFullFileName);

        if (!readFile.open(QIODevice::ReadOnly)) {
            qCritical() << "Failed to open grub file for reading:" << readFile.errorString();
            return;
        }

        //QString strTempFileName = strMountPt + "/EFI/BOOT/tempgrub.cfg";
        QString strTempFileName = strMountPt + "/tempfile";
        QFile writeFile(strTempFileName);

        if (!writeFile.open(QIODevice::ReadWrite|QIODevice::Truncate)) {
            qCritical() << "Failed to open temporary file for writing:" << writeFile.errorString();
            return;
        }

        while (!readFile.atEnd()) {
            QString strData = readFile.readLine();
            QString strData2 = strData.trimmed();

            if ((strData2.startsWith("linux") || strData2.startsWith("append")) && (strData2.contains("inst.stage2")) && (!strData2.contains("inst.repo="))) {
                QString strUUID = XSys::DiskUtil::getPartitionUUID(m_strPartionName);
                strData.remove("\n");
                strData.append(" ");
                strData.append(QString("inst.repo=hd:UUID=%1\n").arg(strUUID));
            }

            writeFile.write(QByteArray::fromStdString(strData.toStdString()));
        }

        readFile.close();
        writeFile.close();
        QFile::remove(strFullFileName);
        QFile::rename(strTempFileName, strFullFileName);
        qInfo() << "Grub file modified successfully";
    } else {
        qWarning() << "Grub file not found:" << strFullFileName;
    }
}
