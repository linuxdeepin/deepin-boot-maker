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
    m_bRunning = true;
    m_bStop = false;
    bool bRet = hasEnoughSpace();

    if (!bRet) {
        checkError();
        return;
    }
    else {
        emit this->reportProgress(5, "check usb space finished", "");
    }

    if (m_bStop) {
        m_bRunning = false;
        return;
    }

    bRet = checkISOIntegrity();

    if (!bRet) {
        checkError();
        return;
    }
    else {
        emit this->reportProgress(10, "check integrity finished", "");
    }

    if (m_bStop) {
        m_bRunning = false;
        return;
    }

    bRet = formatUsb();

    if (!bRet) {
        checkError();
        return;
    }
    else {
        emit this->reportProgress(15, "format usb finished", "");
    }

    if (m_bStop) {
        m_bRunning = false;
        return;
    }

    bRet = installBootload();

    if (!bRet) {
        checkError();
        return;
    }
    else {
        emit this->reportProgress(20, "install bootloader finished", "");
    }

    if (m_bStop) {
        m_bRunning = false;
        return;
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

    if (m_bStop) {
        m_bRunning = false;
        return;
    }

    bRet = syncIO();

    if (!bRet) {
        checkError();
        return;
    }
    else {
        emit this->reportProgress(90, "sync IO finished", "");
    }

    if (m_bStop) {
        m_bRunning = false;
        return;
    }

    configSyslinux();
    //通过判断镜像中是否存在anaconda的文件夹来推测该镜像的安装器

    if(needAddRepo()) {
        qInfo() << "need add repo";
        modifyBootGrubFile("/EFI/BOOT/grub.cfg");
        modifyBootGrubFile("/syslinux/syslinux.cfg");
    } else {
        qInfo() << "not need add repo";
    }

    emit this->reportProgress(100, "finish", "");

    if (m_bStop) {
        m_bRunning = false;
        return;
    }

    bRet = ejectDisk();

    if (!bRet) {
        checkError();
        return;
    }
    else {
        emit this->reportProgress(101, "finish", "");
    }

    m_bRunning = false;
}

void QtBaseInstaller::checkError()
{
    m_bRunning = false;

    if (m_bStop) {
        qInfo() << "Stop Install";
        return;
    }

    qInfo() << "begin check error";
    QString strDisk = XSys::DiskUtil::GetPartitionDisk(m_strPartionName);

    if (strDisk.isEmpty()) {
        qCritical() << "Error::get(Error::USBMountFailed)";
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

bool QtBaseInstaller::isRunning() const
{
    return m_bRunning;
}

void QtBaseInstaller::stopInstall()
{
    m_bStop = true;
    qInfo() << "m_progressStatus:" << m_progressStatus;

    if(EXTRACTISO ==  m_progressStatus || CHECKINTEGRITY == m_progressStatus) {
        qInfo() << "Installer stop install";
        m_sevenZipCheck.stopProcess();
    }
}

bool QtBaseInstaller::hasEnoughSpace()
{
    bool bRet = false;
    qInfo() << "begin check space";
    m_progressStatus = CHECKSPACE;
    QFileInfo isoInfo(m_strImage);
    //分区没有挂载时是获取不到正确的可用空间和分区大小的,因些需要先进行挂载。
    QString strMountPt = XSys::DiskUtil::MountPoint(m_strPartionName);

    if (strMountPt.isEmpty()) {
        XSys::DiskUtil::Mount(m_strPartionName);
    }

    strMountPt = XSys::DiskUtil::MountPoint(m_strPartionName);

    if (strMountPt.isEmpty()) {
        qCritical() << "Can't get correct partition space.";
        return false;
    }

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
    qInfo() << "check iso integrity.";
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

bool QtBaseInstaller::umountPartion()
{
    return XSys::DiskUtil::UmountPartion(m_strPartionName);
}

bool QtBaseInstaller::umountDisk()
{
    bool bRet = false;
    QString device = XSys::DiskUtil::GetPartitionDisk(m_strPartionName);

    if (!XSys::DiskUtil::UmountDisk(device)) {
        qCritical() << "umount partition failed: ";
        bRet = false;
    }
    else {
        bRet = true;
    }

    return bRet;
}

QString QtBaseInstaller::getMountPoint()
{
    return  XSys::DiskUtil::MountPoint(m_strPartionName);
}

bool QtBaseInstaller::ejectDisk()
{
    qInfo() << "begin eject disk";
    m_progressStatus = EJECTDISK;

    if (!(umountDisk())) {
        return false;
    }

    QString strDisk = XSys::DiskUtil::GetPartitionDisk(m_strPartionName);
    XSys::Result result = XSys::DiskUtil::EjectDisk(strDisk);
    return result.isSuccess();
}

bool QtBaseInstaller::formatUsb()
{
    qInfo() << "begin format usb.";
    m_progressStatus = FORMATUSB;

    if (!umountPartion()) {
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
            qDebug() << "Detected disk device" << targetDev << "without partition, creating partition...";
            if (!XSys::DiskUtil::CreatePartition(targetDev)) {
                qCritical() << "Failed to create partition for" << targetDev;
                return false;
            }
            // 使用新创建的分区
            targetDev = targetDev + "1";
            m_strPartionName = targetDev;
            qDebug() << "Partition created successfully, using" << targetDev;
        }

        // 格式化分区
        if (!XSys::DiskUtil::FormatPartion(targetDev)) {
            return false;
        }
    }

    return true;
}

bool QtBaseInstaller::installBootload()
{
    return false;
}

bool QtBaseInstaller::extractISO()
{
    qInfo() << "begin extract ISO to" << m_strPartionName;
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
        XSys::DiskUtil::Mount(m_strPartionName);
        installDir = XSys::DiskUtil::MountPoint(m_strPartionName);
    }

    if (installDir.isEmpty()) {
        qCritical() << "Error::get(Error::USBMountFailed)";
        return false;
    }

    if (m_bStop) {
       return true;
    }

    qInfo() << "begin clear target device files";
    m_progressStatus = EXTRACTISO;
    Utils::ClearTargetDev(installDir);
    m_sevenZipCheck.setArchiveFile(m_strImage);
    m_sevenZipCheck.setOutputDirectory(installDir);
    return m_sevenZipCheck.extract();
}

bool QtBaseInstaller::syncIO()
{
    qInfo() << "begin sysc IO";
    m_progressStatus = SYNCIO;
    return XSys::SynExec("sync", "").isSuccess();
}

bool QtBaseInstaller::configSyslinux()
{
    qInfo() << "begin configure syslinux";
    XSys::SynExec("sync", "");
    QString installDir = XSys::DiskUtil::MountPoint(m_strPartionName);
    qDebug() << "configure syslinux, installDir:" << installDir;
    return XSys::Syslinux::ConfigSyslinx(installDir).isSuccess();
}

bool QtBaseInstaller::needAddRepo()
{
    QStringList args;
    args << "-f" << "-i" << m_strImage;
    XSys::Result ret = XSys::SynExec("isoinfo", args);

    if (!ret.isSuccess()) {
        qWarning() << "call isoinfo failed" << ret.result();
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
    QString strMountPt = XSys::DiskUtil::MountPoint(m_strPartionName);
    QString strFullFileName = strMountPt + grub_file_name;

    if (QFile::exists(strFullFileName)) {
        QFile readFile(strFullFileName);

        if (!readFile.open(QIODevice::ReadOnly)) {
            qCritical() << readFile.errorString();
            return;
        }

        //QString strTempFileName = strMountPt + "/EFI/BOOT/tempgrub.cfg";
        QString strTempFileName = strMountPt + "/tempfile";
        QFile writeFile(strTempFileName);

        if (!writeFile.open(QIODevice::ReadWrite|QIODevice::Truncate)) {
            qCritical() << writeFile.errorString();
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
    }
}
