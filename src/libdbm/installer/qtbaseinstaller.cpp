// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "../util/ddlog.h"
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
    qCDebug(installerLog) << "Initializing QtBaseInstaller";
}

void QtBaseInstaller::setformat(bool bFormat)
{
    qCDebug(installerLog) << "Setting format flag:" << bFormat;
    m_bFormat = bFormat;
}

void QtBaseInstaller::setPartionName(const QString& strPartionName)
{
    qCDebug(installerLog) << "Setting partition name:" << strPartionName;
    m_strPartionName = strPartionName;
}

void QtBaseInstaller::setImage(const QString& strImage)
{
    qCDebug(installerLog) << "Setting image path:" << strImage;
    m_strImage = strImage;
}

void QtBaseInstaller::beginInstall()
{
    qCDebug(installerLog) << "Starting installation process";
    m_bRunning = true;
    m_bStop = false;
    bool bRet = hasEnoughSpace();

    if (!bRet) {
        qCWarning(installerLog) << "Insufficient space check failed";
        checkError();
        return;
    }
    else {
        qCDebug(installerLog) << "USB space check completed successfully";
        emit this->reportProgress(5, "check usb space finished", "");
    }

    if (m_bStop) {
        qCDebug(installerLog) << "Installation stopped by user";
        m_bRunning = false;
        return;
    }

    bRet = checkISOIntegrity();

    if (!bRet) {
        qCWarning(installerLog) << "ISO integrity check failed";
        checkError();
        return;
    }
    else {
        qCDebug(installerLog) << "ISO integrity check completed successfully";
        emit this->reportProgress(10, "check integrity finished", "");
    }

    if (m_bStop) {
        qCDebug(installerLog) << "Installation stopped by user";
        m_bRunning = false;
        return;
    }

    bRet = formatUsb();

    if (!bRet) {
        qCWarning(installerLog) << "USB formatting failed";
        checkError();
        return;
    }
    else {
        qCDebug(installerLog) << "USB formatting completed successfully";
        emit this->reportProgress(15, "format usb finished", "");
    }

    if (m_bStop) {
        qCDebug(installerLog) << "Installation stopped by user";
        m_bRunning = false;
        return;
    }

    bRet = installBootload();

    if (!bRet) {
        qCWarning(installerLog) << "Bootloader installation failed";
        checkError();
        return;
    }
    else {
        qCDebug(installerLog) << "Bootloader installation completed successfully";
        emit this->reportProgress(20, "install bootloader finished", "");
    }

    if (m_bStop) {
        qCDebug(installerLog) << "Installation stopped by user";
        m_bRunning = false;
        return;
    }

    bRet = extractISO();

    if (!bRet) {
        qCWarning(installerLog) << "ISO extraction failed";
        checkError();
        return;
    }
    else {
        qCDebug(installerLog) << "ISO extraction completed successfully";
        emit this->reportProgress(80, "extract ISO finished", "");
    }

    qCDebug(installerLog) << "Starting IO synchronization";
    emit this->reportProgress(81, "begin sync IO", "");

    if (m_bStop) {
        qCInfo(installerLog) << "Installation stopped by user";
        m_bRunning = false;
        return;
    }

    bRet = syncIO();

    if (!bRet) {
        qCWarning(installerLog) << "IO synchronization failed";
        checkError();
        return;
    }
    else {
        qCDebug(installerLog) << "IO synchronization completed successfully";
        emit this->reportProgress(90, "sync IO finished", "");
    }

    if (m_bStop) {
        qCDebug(installerLog) << "Installation stopped by user";
        m_bRunning = false;
        return;
    }

    qCInfo(installerLog) << "Configuring syslinux";
    configSyslinux();
    //通过判断镜像中是否存在anaconda的文件夹来推测该镜像的安装器

    if(needAddRepo()) {
        qCInfo(installerLog) << "Repository configuration needed - modifying boot files";
        modifyBootGrubFile("/EFI/BOOT/grub.cfg");
        modifyBootGrubFile("/syslinux/syslinux.cfg");
    } else {
        qCInfo(installerLog) << "No repository configuration needed";
    }

    qCInfo(installerLog) << "Installation process completed";
    emit this->reportProgress(100, "finish", "");

    if (m_bStop) {
        qCInfo(installerLog) << "Installation stopped by user";
        m_bRunning = false;
        return;
    }

    bRet = ejectDisk();

    if (!bRet) {
        qCWarning(installerLog) << "Disk ejection failed";
        checkError();
        return;
    }
    else {
        qCInfo(installerLog) << "Disk ejected successfully";
        emit this->reportProgress(101, "finish", "");
    }

    qCInfo(installerLog) << "Installation process finished";
    m_bRunning = false;
}

void QtBaseInstaller::checkError()
{
    qCInfo(installerLog) << "Checking installation error";
    m_bRunning = false;

    if (m_bStop) {
        qCInfo(installerLog) << "Installation stopped by user";
        return;
    }

    qCInfo(installerLog) << "Beginning error check";
    QString strDisk = XSys::DiskUtil::GetPartitionDisk(m_strPartionName);

    if (strDisk.isEmpty()) {
        qCCritical(installerLog) << "Failed to get partition disk information";
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
            qCCritical(installerLog) << "Partition not found in disk partitions";
            emit progressfinished(m_progressStatus, BMHandler::ErrorType::USBMountFailed);
            return;
        }
    }

    if (m_progressStatus == CHECKSPACE) {
        qCCritical(installerLog) << "Error::get(Error::USBSizeError)";
        emit progressfinished(m_progressStatus, BMHandler::ErrorType::USBSizeError);
        return;
    }
    else if (m_progressStatus == CHECKINTEGRITY) {
        qCCritical(installerLog) << "Error::get(Error::CheckImageIntegrityFailed)";
        emit progressfinished(m_progressStatus, BMHandler::ErrorType::CheckImageIntegrityFailed);
        return;
    }
    else if (m_progressStatus == FORMATUSB) {
        qCCritical(installerLog) << "Error::get(Error::USBFormatError)";
        emit progressfinished(m_progressStatus, BMHandler::ErrorType::USBFormatError);
    }
    else if (m_progressStatus == INSTALLBOOTLOADER) {
        qCCritical(installerLog) << "Error::get(Error::InstallBootloaderFailed)";
        emit progressfinished(m_progressStatus, BMHandler::ErrorType::InstallBootloaderFailed);
    }
    else if (m_progressStatus == GETINSTALLDIR) {
        qCCritical(installerLog) << "Error::get(Error::GetUsbInstallDirFailed)";
        emit progressfinished(m_progressStatus, BMHandler::ErrorType::GetUsbInstallDirFailed);
    }
    else if (m_progressStatus == EXTRACTISO) {
        qCCritical(installerLog) << "Error::get(Error::ExtractImgeFailed)";
        emit progressfinished(m_progressStatus, BMHandler::ErrorType::ExtractImgeFailed);
    }
    else if (m_progressStatus == SYNCIO) {
        qCCritical(installerLog) << "Error::get(Error::SyncIOFailed)";
        emit progressfinished(m_progressStatus, BMHandler::ErrorType::SyncIOFailed);
    }
    else {
        qCCritical(installerLog) << "Error::get(Error::UnDefinedError)";
        emit progressfinished(m_progressStatus, BMHandler::ErrorType::UnDefinedError);
    }
}

bool QtBaseInstaller::isRunning() const
{
    return m_bRunning;
}

void QtBaseInstaller::stopInstall()
{
    qCInfo(installerLog) << "Stopping installation process - Current status:" << m_progressStatus;
    m_bStop = true;

    if(EXTRACTISO == m_progressStatus || CHECKINTEGRITY == m_progressStatus) {
        qCInfo(installerLog) << "Stopping SevenZip process";
        m_sevenZipCheck.stopProcess();
    }
}

bool QtBaseInstaller::hasEnoughSpace()
{
    qCInfo(installerLog) << "Checking available space";
    m_progressStatus = CHECKSPACE;
    QFileInfo isoInfo(m_strImage);
    //分区没有挂载时是获取不到正确的可用空间和分区大小的,因些需要先进行挂载。
    QString strMountPt = XSys::DiskUtil::MountPoint(m_strPartionName);

    if (strMountPt.isEmpty()) {
        qCInfo(installerLog) << "Mounting partition for space check";
        XSys::DiskUtil::Mount(m_strPartionName);
    }

    strMountPt = XSys::DiskUtil::MountPoint(m_strPartionName);

    if (strMountPt.isEmpty()) {
        qCCritical(installerLog) << "Failed to get mount point for space check";
        return false;
    }

#define KByt 1024
    if (m_bFormat) {
        if (isoInfo.size() / KByt > XSys::DiskUtil::GetPartitionTotalSpace(m_strPartionName)) {
            qCCritical(installerLog) << "Insufficient total space on partition";
            return false;
        }
        else {
            qCInfo(installerLog) << "Sufficient total space available";
            return true;
        }
    } else {
        if (isoInfo.size() / KByt > XSys::DiskUtil::GetPartitionFreeSpace(m_strPartionName)) {
            qCCritical(installerLog) << "Insufficient free space on partition";
            return false;
        }
        else {
            qCInfo(installerLog) << "Sufficient free space available";
            return true;
        }
    }
}

bool QtBaseInstaller::checkISOIntegrity()
{
    qCInfo(installerLog) << "Checking ISO integrity";
    m_progressStatus = CHECKINTEGRITY;

    m_sevenZipCheck.setArchiveFile(m_strImage);
    m_sevenZipCheck.setOutputDirectory("");

    if (!m_sevenZipCheck.check()) {
        qCCritical(installerLog) << "ISO integrity check failed";
        return false;
    }
    else {
        qCInfo(installerLog) << "ISO integrity check passed";
        return true;
    }
}

bool QtBaseInstaller::umountPartion()
{
    qCInfo(installerLog) << "Unmounting partition:" << m_strPartionName;
    return XSys::DiskUtil::UmountPartion(m_strPartionName);
}

bool QtBaseInstaller::umountDisk()
{
    qCInfo(installerLog) << "Unmounting disk";
    bool bRet = false;
    QString device = XSys::DiskUtil::GetPartitionDisk(m_strPartionName);

    if (!XSys::DiskUtil::UmountDisk(device)) {
        qCCritical(installerLog) << "Failed to unmount disk:" << device;
        bRet = false;
    }
    else {
        qCInfo(installerLog) << "Disk unmounted successfully";
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
    qCInfo(installerLog) << "Ejecting disk";
    m_progressStatus = EJECTDISK;

    if (!(umountDisk())) {
        qCCritical(installerLog) << "Failed to unmount disk before ejection";
        return false;
    }

    QString strDisk = XSys::DiskUtil::GetPartitionDisk(m_strPartionName);
    XSys::Result result = XSys::DiskUtil::EjectDisk(strDisk);
    return result.isSuccess();
}

bool QtBaseInstaller::formatUsb()
{
    qCInfo(installerLog) << "Formatting USB device";
    m_progressStatus = FORMATUSB;

    if (!umountPartion()) {
        qCCritical(installerLog) << "Failed to unmount partition before formatting";
        return false;
    }

    if (m_bFormat) {
        QString targetDev = m_strPartionName;

        // 检查是否是 disk 节点（没有分区号）
        // disk 节点如：/dev/sdb
        // partition 节点如：/dev/sdb1
        bool isDisk = true;
        for (int i = targetDev.length() - 1; i >= 0; i--) {
            if (targetDev[i].isDigit()) {
                isDisk = false;
                break;
            }
            if (targetDev[i] == '/') {
                break;
            }
        }

        // 如果是 disk 节点，需要先创建分区
        if (isDisk) {
            qCDebug(installerLog) << "Detected disk device" << targetDev << "without partition, creating partition...";
            if (!XSys::DiskUtil::CreatePartition(targetDev)) {
                qCCritical(installerLog) << "Failed to create partition for" << targetDev;
                return false;
            }
            // 使用新创建的分区
            targetDev = targetDev + "1";
            m_strPartionName = targetDev;
            qCDebug(installerLog) << "Partition created successfully, using" << targetDev;
        }

        // 格式化分区
        if (!XSys::DiskUtil::FormatPartion(targetDev)) {
            return false;
        }
        qCInfo(installerLog) << "Partition formatted successfully";
    }

    return true;
}

bool QtBaseInstaller::installBootload()
{
    qCInfo(installerLog) << "Installing bootloader";
    return false;
}

bool QtBaseInstaller::extractISO()
{
    qCInfo(installerLog) << "Extracting ISO to partition:" << m_strPartionName;
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
        qCInfo(installerLog) << "Mounting partition for extraction";
        XSys::DiskUtil::Mount(m_strPartionName);
        installDir = XSys::DiskUtil::MountPoint(m_strPartionName);
    }

    if (installDir.isEmpty()) {
        qCCritical(installerLog) << "Failed to get mount point for extraction";
        return false;
    }

    if (m_bStop) {
        qCInfo(installerLog) << "Extraction stopped by user";
        return true;
    }

    qCInfo(installerLog) << "Clearing target device files";
    m_progressStatus = EXTRACTISO;
    Utils::ClearTargetDev(installDir);
    m_sevenZipCheck.setArchiveFile(m_strImage);
    m_sevenZipCheck.setOutputDirectory(installDir);
    return m_sevenZipCheck.extract();
}

bool QtBaseInstaller::syncIO()
{
    qCInfo(installerLog) << "Synchronizing IO";
    m_progressStatus = SYNCIO;
    return XSys::SynExec("sync", "").isSuccess();
}

bool QtBaseInstaller::configSyslinux()
{
    qCInfo(installerLog) << "Configuring syslinux";
    XSys::SynExec("sync", "");
    QString installDir = XSys::DiskUtil::MountPoint(m_strPartionName);
    qCDebug(installerLog) << "Configuring syslinux in directory:" << installDir;
    bool result = XSys::Syslinux::ConfigSyslinx(installDir).isSuccess();
    if (result) {
        qCInfo(installerLog) << "Syslinux configuration completed successfully";
    } else {
        qCCritical(installerLog) << "Syslinux configuration failed";
    }
    return result;
}

bool QtBaseInstaller::needAddRepo()
{
    qCInfo(installerLog) << "Checking if repository configuration is needed";
    QStringList args;
    args << "-f" << "-i" << m_strImage;
    XSys::Result ret = XSys::SynExec("isoinfo", args);

    if (!ret.isSuccess()) {
        qCWarning(installerLog) << "Failed to execute isoinfo:" << ret.result();
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
    qCInfo(installerLog) << "Modifying boot grub file:" << grub_file_name;
    QString strMountPt = XSys::DiskUtil::MountPoint(m_strPartionName);
    QString strFullFileName = strMountPt + grub_file_name;

    if (QFile::exists(strFullFileName)) {
        QFile readFile(strFullFileName);

        if (!readFile.open(QIODevice::ReadOnly)) {
            qCCritical(installerLog) << "Failed to open grub file for reading:" << readFile.errorString();
            return;
        }

        //QString strTempFileName = strMountPt + "/EFI/BOOT/tempgrub.cfg";
        QString strTempFileName = strMountPt + "/tempfile";
        QFile writeFile(strTempFileName);

        if (!writeFile.open(QIODevice::ReadWrite|QIODevice::Truncate)) {
            qCCritical(installerLog) << "Failed to open temporary file for writing:" << writeFile.errorString();
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
        qCInfo(installerLog) << "Grub file modified successfully";
    } else {
        qCWarning(installerLog) << "Grub file not found:" << strFullFileName;
    }
}
