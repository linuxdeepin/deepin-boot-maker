// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "DiskUtil.h"

#include "../FileSystem/FileSystem.h"
#include "../Cmd/Cmd.h"
#include "Syslinux.h"

#include <QtCore>
#include <QString>
#include <QSysInfo>
#include <QRegularExpression>

#ifdef Q_OS_WIN32
#include <Windows.h>
#include <winioctl.h>
#include <shellapi.h>

#endif

namespace XAPI {

#ifdef Q_OS_WIN32


BOOL GetStorageDeviceNumberByHandle(HANDLE handle,
                                    const STORAGE_DEVICE_NUMBER *sdn)
{
    BOOL result = FALSE;
    DWORD count;

    if (DeviceIoControl(handle, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0,
                        (LPVOID)sdn, sizeof(*sdn), &count, NULL)) {
        result = TRUE;

    } else {
        qWarning("GetDriveNumber: DeviceIoControl failed");
    }

    return (result);
}

int GetPartitionDiskNum(QString targetDev)
{
    QString driverName = "\\\\.\\" + targetDev.remove('\\');
    WCHAR wdriverName[1024] = { 0 };
    driverName.toWCharArray(wdriverName);
    HANDLE handle = CreateFile(wdriverName, GENERIC_READ | GENERIC_WRITE,
                               FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                               OPEN_EXISTING, 0, NULL);

    if (handle == INVALID_HANDLE_VALUE) {
        qWarning() << "Open Dev Failed: " << driverName << GetLastError();
        return -1;
    }

    STORAGE_DEVICE_NUMBER sdn;

    if (GetStorageDeviceNumberByHandle(handle, &sdn)) {
        CloseHandle(handle);
        return sdn.DeviceNumber;
    }

    CloseHandle(handle);
    return -1;
}

QString GetPartitionLabel(const QString &targetDev)
{
    QString devname = QString("%1:\\").arg(targetDev.at(0));
    WCHAR wLabel[1024] = { 0 };
    if (!GetVolumeInformation(LPWSTR(devname.utf16()), wLabel, 1023, NULL, NULL, NULL, NULL, 0)) {
        return "";
    }
    return QString("").fromWCharArray(wLabel);
}

qint64 GetPartitionFreeSpace(const QString &targetDev)
{
    QString devname = QString("%1:\\").arg(targetDev.at(0));
    ULARGE_INTEGER FreeAv, TotalBytes, FreeBytes;
    if (!GetDiskFreeSpaceEx(LPWSTR(devname.utf16()), &FreeAv, &TotalBytes, &FreeBytes)) {
        return 0;
    }
    return FreeBytes.QuadPart;
}

BOOL GetPartitionByHandle(HANDLE handle, PARTITION_INFORMATION &partInfo)
{
    DWORD count;
    BOOL result = DeviceIoControl(
                      (HANDLE) handle,                 // handle to a partition
                      IOCTL_DISK_GET_PARTITION_INFO, // dwIoControlCode
                      NULL,                             // lpInBuffer
                      0,                                // nInBufferSize
                      (LPVOID) &partInfo,             // output buffer
                      (DWORD) sizeof(partInfo),           // size of output buffer
                      (LPDWORD) &count,        // number of bytes returned
                      (LPOVERLAPPED) NULL       // OVERLAPPED structure
                  );
    if (FALSE == result) {
        qWarning() << "IOCTL_DISK_GET_PARTITION_INFO Failed" << GetLastError();
    }
    return result;
}


/*
   return physic driver name like "\\\\.\\PHYSICALDRIVE1";
   */
QString GetPartitionDisk(const QString &targetDev)
{
    QString physicalDevName;
    int deviceNum = GetPartitionDiskNum(targetDev);

    if (-1 != deviceNum) {
        physicalDevName = QString("\\\\.\\PHYSICALDRIVE%1").arg(deviceNum);
    }

    return physicalDevName;
}

BOOL PreventRemovalOfDisk(HANDLE handle, BOOLEAN preventRemoval)
{
    DWORD dwBytesReturned;
    PREVENT_MEDIA_REMOVAL PMRBuffer;

    PMRBuffer.PreventMediaRemoval = preventRemoval;

    return DeviceIoControl(handle,
                           IOCTL_STORAGE_MEDIA_REMOVAL,
                           &PMRBuffer,
                           sizeof(PREVENT_MEDIA_REMOVAL),
                           NULL, 0,
                           &dwBytesReturned,
                           NULL);
}

BOOL AutoEjectDisk(HANDLE handle)
{
    DWORD dwBytesReturned;
    return DeviceIoControl(handle,
                           IOCTL_STORAGE_EJECT_MEDIA,
                           NULL,
                           0,
                           NULL,
                           0,
                           &dwBytesReturned,
                           NULL);
}

HANDLE OpenDisk(const QString &targetDev)
{
    HANDLE handle;
    TCHAR szVolumeName[8];
    TCHAR driveLetter[2];
    targetDev.left(2).toWCharArray(driveLetter);

    wsprintf(szVolumeName,  TEXT("\\\\.\\%c:"), driveLetter[0]);

    handle = CreateFile(szVolumeName,
                        GENERIC_READ | GENERIC_WRITE,
                        FILE_SHARE_READ | FILE_SHARE_WRITE,
                        NULL,
                        OPEN_EXISTING,
                        0,
                        NULL);
    return handle;
}

BOOL CloseDisk(HANDLE handle)
{
    return CloseHandle(handle);
}

BOOL LockDisk(HANDLE handle)
{
    if (!DeviceIoControl(handle, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, NULL, 0)) {
        CloseDisk(handle);
        return FALSE;
    }

    return TRUE;
}

void UnlockDisk(HANDLE handle)
{
    DeviceIoControl(handle, IOCTL_DISK_UPDATE_PROPERTIES, NULL, 0, NULL, 0, NULL, 0);
    DeviceIoControl(handle, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, NULL, 0);
}

BOOL DismountDisk(HANDLE handle)
{
    DWORD dwBytesReturned;
    return DeviceIoControl(handle,
                           FSCTL_DISMOUNT_VOLUME,
                           NULL,
                           0,
                           NULL,
                           0,
                           &dwBytesReturned,
                           NULL);
}

BOOL EjectDisk(const QString &targetDev)
{
    HANDLE handle = OpenDisk(targetDev);
    if (handle == INVALID_HANDLE_VALUE) {
        return FALSE;
    }

    if (LockDisk(handle) && DismountDisk(handle)) {
        // Set prevent removal to false and eject the volume.
        if (PreventRemovalOfDisk(handle, FALSE) &&  AutoEjectDisk(handle)) {
            qDebug() << "disk has been auto ejected";
        }
    }

    if (!CloseDisk(handle)) {
        return FALSE;
    }

    return TRUE;
}

XSys::Result InstallSyslinux(const QString &targetDev)
{
    // install syslinux
    XSys::SynExec("label", QString(" %1:UOS").arg(targetDev[0]));
    QString sysliuxPath = XSys::FS::InsertTmpFile(QString(":/blob/syslinux/win32/syslinux.exe"));
    return XSys::SynExec(sysliuxPath, QString(" -i -m -a %1:").arg(targetDev[0]));
}

XSys::Result InstallBootloader(const QString &targetDev)
{
    qDebug() << "FixUsbDisk Begin! " << targetDev;
    int deviceNum = GetPartitionDiskNum(targetDev);
    QString xfbinstDiskName = QString("(hd%1)").arg(deviceNum);

    // HANDLE handle = LockDisk(targetDev);
    // fbinst format
    QString xfbinstPath = XSys::FS::InsertTmpFile(QString(":/blob/xfbinst/xfbinst.exe"));
    XSys::Result ret = XSys::SynExec(xfbinstPath, QString(" %1 format --fat32 --align --force")
                                     .arg(xfbinstDiskName));
    if (!ret.isSuccess()) {
        return ret;
    }

    // install fg.cfg
    QString tmpfgcfgPath = XSys::FS::InsertTmpFile(QString(":/blob/xfbinst/fb.cfg"));
    XSys::SynExec(xfbinstPath, QString(" %1 add-menu fb.cfg \"%2\" ")
                  .arg(xfbinstDiskName)
                  .arg(tmpfgcfgPath));

    QString driverLetter = QString("%1:").arg(targetDev[0]);
    XSys::SynExec("label", QString("%1 UOS").arg(driverLetter));

    // install syslinux
    QString sysliuxPath = XSys::FS::InsertTmpFile(QString(":/blob/syslinux/win32/syslinux.exe"));
    XSys::SynExec(sysliuxPath, QString(" -i %1").arg(driverLetter));

    // get pbr file ldlinux.bin
    qDebug() << "dump pbr begin";
    QString tmpPbrPath = XSys::FS::TmpFilePath("ldlinux.bin");
    QFile pbr(tmpPbrPath);
    if (!pbr.open(QIODevice::WriteOnly)) {
        qDebug() << "error open file: " << pbr.fileName();
        return XSys::Result(XSys::Result::Failed, pbr.errorString());
    }

    QString targetPhyName = "\\\\.\\" + QString(targetDev).remove('\\');
    QFile targetPhy(targetPhyName);
    if (!targetPhy.open(QIODevice::ReadOnly)) {
        qDebug() << "error open file: " << targetPhy.fileName();
        pbr.close();
        return XSys::Result(XSys::Result::Failed, targetPhy.errorString());
    }

    pbr.write(targetPhy.read(512));
    targetPhy.close();
    pbr.close();
    qDebug() << "dump pbr end";

    // add pbr file ldlinux.bin
    XSys::SynExec(xfbinstPath, QString(" %1 add ldlinux.bin \"%2\" -s")
                  .arg(xfbinstDiskName)
                  .arg(tmpPbrPath));

    return XSys::Result(XSys::Result::Success, "", targetDev);
}

XSys::Result UmountDisk(const QString &targetDev)
{
    if (targetDev.isEmpty()) {
        return XSys::Result(XSys::Result::Failed, "empty target dev");
    }

    if (EjectDisk(targetDev)) {
        return XSys::Result(XSys::Result::Success, "");
    } else {
        qDebug() << "UmountDisk failed" << targetDev;
        return XSys::Result(XSys::Result::Failed, "Umount disk failed");
    }
}

bool CheckFormatFat32(const QString &targetDev)
{
    XSys::Result result = XSys::SynExec("cmd",  QString("/C \"chcp 437 & fsutil fsinfo volumeinfo %1\" ").arg(targetDev));

    if (result.result().contains("File System Name : FAT32", Qt::CaseInsensitive)) {
        return true;
    }

    return false;
}

const QString MountPoint(const QString &targetDev)
{
    return QString(targetDev).remove("/").remove("\\") + "/";
}

bool Mount(const QString &/*targetDev*/, const QString &/*path*/)
{
    return true;
}

#endif

#ifdef Q_OS_UNIX
const QString MountPoint(const QString &targetDev)
{
    /*
    文件系统       挂载点
    udev           /dev
    tmpfs          /run
    */
    XSys::Result ret = XSys::SynExec("df", "--output=source,target");

    if (!ret.isSuccess()) {
        qWarning() << "call df failed" << ret.result();
    }

    QStringList mounts = ret.result().split("\n").filter(targetDev);

    if (0 == mounts.size()) {
        return "";
    }

    QString mountinfo = mounts.last();
    mountinfo.remove(targetDev);
    return mountinfo.mid(mountinfo.indexOf('/'));
}


bool Mount(const QString &targetDev, const QString &path)
{
    XSys::SynExec("mkdir", QString(" -p %1").arg(path));
    XSys::Result ret = XSys::SynExec("mount", QString(" %1 %2").arg(targetDev).arg(path));
    return ret.isSuccess();
}

#endif

#ifdef Q_OS_LINUX

QString GetPartitionDisk(QString targetDev)
{
    if (targetDev.contains(QRegularExpression("p\\d$"))) {
        return QString(targetDev).remove(QRegularExpression("p\\d$"));
    } else {
        return QString(targetDev).remove(QRegularExpression("\\d$"));
    }
}

bool CheckFormatFat32(const QString &targetDev)
{
    XSys::Result ret = XSys::SynExec("blkid", "-s TYPE " + targetDev);
    if (ret.isSuccess() && ret.result().contains("vfat", Qt::CaseInsensitive)) {
        return true;
    }
    return false;
}

QString GetPartitionLabel(const QString &targetDev)
{
    XSys::Result ret = XSys::SynExec("blkid", "-s LABEL -o value " + targetDev);
    if (!ret.isSuccess()) {
        return "";
    }
    return ret.result();
}

qint64 GetPartitionTotalSpace(const QString &targetDev)
{
    XSys::Result ret = XSys::SynExec("df", "--output=size " + targetDev);
    if (!ret.isSuccess()) {
        return 0;
    }

    qDebug() << "result" << ret.result().split("\n").at(ret.result().split("\n").count() - 2);
    return ret.result().split("\n").at(ret.result().split("\n").count() - 2).toLongLong();
}

qint64 GetPartitionFreeSpace(const QString &targetDev)
{
    XSys::Result ret = XSys::SynExec("df", "--output=avail " + targetDev);
    if (!ret.isSuccess()) {
        return 0;
    }

    qDebug() << "result" << ret.result().split("\n").at(ret.result().split("\n").count() - 2);
    return ret.result().split("\n").at(ret.result().split("\n").count() - 2).toLongLong();
}

XSys::Result InstallSyslinux(const QString &targetDev, const QString &images)
{
    // install syslinux
    QString rawtargetDev = GetPartitionDisk(targetDev);
    XSys::Result ret = XSys::Syslinux::InstallBootloader(targetDev);
    if (!ret.isSuccess()) {
        return ret;
    }
    ret = XSys::Syslinux::InstallMbr(rawtargetDev);
    if (!ret.isSuccess()) {
        return ret;
    }

    // make active
    ret = XSys::SynExec(XSys::FS::SearchBin("sfdisk"),
                        QString("%1 -A %2").arg(rawtargetDev, QString(targetDev).remove(rawtargetDev).remove("p")));
    if (!ret.isSuccess()) {
        return ret;
    }

    // rename label
    ret = XSys::SynExec(XSys::FS::SearchBin("fsck"), QString("-y %1").arg(targetDev));
    /*QProcess cannot handle Chinese paths*/
    QStringList args;
    args << "-i" << images << "-d";
    XSys::Result ret3 = XSys::SynExec("isoinfo", args);
    if (!ret3.isSuccess()) {
        qWarning() << "call isoinfo failed" << ret3.result();
    }
    QStringList volume = ret3.result().split("\n").filter("Volume id");
    QString tem = volume.takeAt(0);
    if (tem.contains("Volume id: deepin", Qt::CaseInsensitive)) {
        ret = XSys::SynExec(XSys::FS::SearchBin("fatlabel"), QString(" %1 DEEPINOS").arg(targetDev));
    } else if (tem.contains("Volume id: uos", Qt::CaseInsensitive)) {
        ret = XSys::SynExec(XSys::FS::SearchBin("fatlabel"), QString(" %1 UOS").arg(targetDev));
    } else {
        ret = XSys::SynExec(XSys::FS::SearchBin("fatlabel"), QString(" %1 UNKNOWN").arg(targetDev));
    }

    XSys::DiskUtil::Mount(targetDev);
    return ret;
}

#endif


#ifdef Q_OS_MAC
QString removAroundSpace(QString raw)
{
    QString out;
    for (int i = 0; i < raw.length(); ++i) {
        if (' ' != raw.at(i)) {
            out = raw.right(raw.length() - i);
            break;
        }
    }
    raw = out;
    for (int i = 0; i < raw.length(); ++i) {
        if (' ' != raw.at(raw.length() - i)) {
            out = raw.left(raw.length() - i);
            break;
        }
    }
    return out;
}


QString GetPartitionLabel(const QString &targetDev)
{
    XSys::Result ret = XSys::SynExec("diskutil",  "info " + targetDev);
    if (!ret.isSuccess()) {
        qDebug() << "Call df Failed";
        return "";
    }
    QString r = ret.result().split("\n").filter("Volume Name:").first().remove("Volume Name:");

    return removAroundSpace(r);
}

qint64 GetPartitionFreeSpace(const QString &targetDev)
{
    XSys::Result ret = XSys::SynExec("df", "-b");
    if (!ret.isSuccess()) {
        qDebug() << "Call df Failed";
        return 0;
    }
    return ret.result().split("\n").filter(targetDev).first().split(" ").filter(QRegularExpression("[^\\s]")).at(3).toLongLong() * 512;
}

QString GetPartitionDisk(QString targetDev)
{
    return QString(targetDev).remove(QRegularExpression("s\\d$"));
}

XSys::Result UmountDisk(const QString &targetDev)
{
    return XSys::SynExec("diskutil", QString("unmountDisk force %1").arg(GetPartitionDisk(targetDev)));
}

bool CheckFormatFat32(const QString &targetDev)
{
    XSys::Result ret = XSys::SynExec("diskutil", "info " + targetDev);
    QString partitionType = ret.result().split("\n").filter("Partition Type:").first();

    if (partitionType.contains(QRegularExpression("_FAT_32"))) {
        return true;
    }

    return false;
}


QString Resource(const QString &name)
{
    QDir res = QDir(QCoreApplication::applicationDirPath());
    res.cdUp();
    res.cd("Resources");
    return res.absoluteFilePath(name);
}

XSys::Result InstallSyslinux(const QString &targetDev)
{
    // rename to DEEPINOS
    XSys::SynExec("diskutil", QString("rename %1 UOS").arg(targetDev));

    // install syslinux
    UmountDisk(targetDev);
    QString sysliuxPath = Resource("syslinux-mac");
    XSys::SynExec(sysliuxPath, QString(" -i %1").arg(targetDev));

    // dd pbr file ldlinux.bin
    UmountDisk(targetDev);
    QString tmpPbrPath = XSys::FS::InsertTmpFile(Resource("mbr.bin"));
    XSys::SynExec("dd", QString(" if=%1 of=%2 ").arg(tmpPbrPath).arg(
                      GetPartitionDisk(targetDev)));
    UmountDisk(targetDev);

    QThread::sleep(2);

    return XSys::SynExec("diskutil", QString("mount %1").arg(targetDev));
}

static void SureUmount(const QString &targetDev)
{
    UmountDisk(targetDev);
    QThread::sleep(1);
    UmountDisk(targetDev);
    QThread::sleep(1);
}

XSys::Result InstallBootloader(const QString &diskDev)
{
    QString targetDev = diskDev + "s1";
    QString xfbinstDiskName = QString("\"(hd%1)\"").arg(diskDev[diskDev.length() - 1]);
    // format with xfbinst
    QString xfbinstPath = Resource("xfbinst");

    SureUmount(targetDev);
    XSys::SynExec(xfbinstPath, QString(" %1 format --fat32 --align --force").arg(xfbinstDiskName));

    // install fg.cfg
    QString tmpfgcfgPath = XSys::FS::InsertTmpFile(QString(":/blob/xfbinst/fb.cfg"));

    SureUmount(targetDev);
    XSys::SynExec(xfbinstPath, QString(" %1 add-menu fb.cfg %2 ").arg(xfbinstDiskName).arg(tmpfgcfgPath));


    // rename to DEEPINOS
    XSys::SynExec("diskutil", QString("mountDisk %1").arg(diskDev));
    XSys::SynExec("diskutil", QString("rename %1 UOS").arg(targetDev));

    // install syslinux
    SureUmount(targetDev);

    QString sysliuxPath = Resource("syslinux-mac");
    UmountDisk(targetDev);
    XSys::SynExec(sysliuxPath, QString(" -i %1").arg(targetDev));

    // dd pbr file ldlinux.bin
    QString tmpPbrPath = XSys::FS::TmpFilePath("ldlinux.bin");
    UmountDisk(targetDev);
    XSys::SynExec("dd", QString(" if=%1 of=%2 bs=512 count=1").arg(targetDev).arg(tmpPbrPath));

    // add pbr file ldlinux.bin
    UmountDisk(targetDev);
    XSys::SynExec(xfbinstPath, QString(" %1 add ldlinux.bin %2 -s").arg(xfbinstDiskName).arg(tmpPbrPath));

    XSys::SynExec("diskutil", QString("mountDisk %1").arg(diskDev));

    return XSys::Result(XSys::Result::Success, "", targetDev);
}

#endif
}

// End XAPI
// //////////////////////////////////////
namespace XSys {
namespace DiskUtil {

PartionFormat GetPartitionFormat(const QString &targetDev)
{
    if (XAPI::CheckFormatFat32(targetDev)) {
        return PF_FAT32;
    }
    return PF_RAW;
}

QString GetPartitionLabel(const QString &targetDev)
{
    return XAPI::GetPartitionLabel(targetDev);
}

bool FormatPartion(const QString& targetDev)
{
    return XSys::SynExec("mkfs.fat", targetDev).isSuccess();
}

QStringList GetPartionOfDisk(const QString& strDisk)
{
    QStringList strPartions;
    XSys::Result result = XSys::SynExec("lsblk", "-P -o name,type");

    if (result.isSuccess()) {
        QString strResult = result.result();
        QStringList strLines = strResult.split("\n");

        foreach (QString strLine, strLines) {
            if (strLine.contains(strDisk)) {
                QStringList strPairs = strLine.split(" ");

                if (strPairs.size() != 2) {
                    strPartions.clear();
                    break;
                }

                QString strName;
                QString strType;

                foreach (QString strPair, strPairs) {
                    QStringList strValues = strPair.split("=");

                    if (0 == strValues[0].compare("NAME", Qt::CaseInsensitive)) {
                        strName = strValues[1];
                    }
                    else {
                        strType = strValues[1];
                    }
                }

                strName = strName.remove("\"");
                strType = strType.remove("\"");

                if((strName == strDisk) &&(strType != "disk")) {
                    strPartions.clear();
                    break;
                }
                else {
                    if (strType == "part") {
                        strPartions.push_back(strName);
                    }
                }
            }
        }
    }

    return strPartions;
}

bool SetActivePartion(const QString& strDisk, const QString& strPartion)
{
    QString strTemp = strPartion;
    QString strIndex = strTemp.remove(strDisk);
    XSys::Result ret = XSys::SynExec(XSys::FS::SearchBin("sfdisk"), QString("%1 -A %2").arg(strDisk).arg(strIndex));
    return ret.isSuccess();
}

void SetPartionLabel(const QString& strPartion, const QString& strImage)
{
    /*QProcess cannot handle Chinese paths*/
    XSys::SynExec(XSys::FS::SearchBin("fsck"), QString("-y %1").arg(strPartion));
    QStringList args;
    args << "-i" << strImage << "-d";
    XSys::Result ret3 = XSys::SynExec("isoinfo", args);

    if (!ret3.isSuccess()) {
        qWarning() << "call isoinfo failed" << ret3.result();
        return;
    }

    QStringList volume = ret3.result().split("\n").filter("Volume id");
    QString tem = volume.takeAt(0);
    QStringList strValues = tem.split(":");
    QString strName = QString("UNKNOWN");
    QString strTemp;

    if (2 == strValues.size()) {
        strTemp = strValues.at(1);
        strTemp = strTemp.trimmed();
    }

    //标签名最长长度为十一位
    strName = strTemp.isEmpty()?strName:(strTemp.length() > 11)?strTemp.left(11):strTemp;
    XSys::SynExec(XSys::FS::SearchBin("fatlabel"), QString(" %1 \"%2\"").arg(strPartion).arg(strName));
}

QString getPartitionUUID(const QString& strPartition)
{
    XSys::Result ret = XSys::SynExec("lsblk", "-p -l --fs -o name,uuid");

    if (!ret.isSuccess()) {
        qWarning() << "call lsblk failed" << ret.result();
    }

    QStringList uuids = ret.result().split("\n").filter(strPartition);

    if (0 == uuids.size()) {
        return "";
    }

    QString strLine = uuids.last();
    strLine  = strLine.remove(strPartition);
    QString strUuid = strLine.trimmed();
    return strUuid;
}

qint64 GetPartitionTotalSpace(const QString &targetDev)
{
    return XAPI::GetPartitionTotalSpace(targetDev);
}

qint64 GetPartitionFreeSpace(const QString &targetDev)
{
    return XAPI::GetPartitionFreeSpace(targetDev);
}

QString GetPartitionDisk(const QString &targetDev)
{
    return XAPI::GetPartitionDisk(targetDev);
}

XSys::Result EjectDisk(const QString &targetDev)
{
    return XSys::SynExec("udisksctl", QString("power-off -b %1").arg(GetPartitionDisk(targetDev)));
}

bool UmountPartion(const QString& strPartionName)
{
    bool bRet = false;
    //对一个分区进行unmount,最多尝试5次。
    int iCount= 5;
    QString strMountPt = XSys::DiskUtil::MountPoint(strPartionName);

    if (!strMountPt.isEmpty()) {
        do {
            XSys::Result result = XSys::SynExec("udisksctl", QString("unmount -b %1").arg(strPartionName));

            if (!result.isSuccess()) {
                bRet = false;
            }

            strMountPt = XSys::DiskUtil::MountPoint(strPartionName);

            if (strMountPt.isEmpty()) {
                bRet = true;
                break;
            }
            QThread::sleep(3);
        } while (--iCount > 0);
    }
    else {
        bRet = true;
    }

    //经测试在服务器版上执行udiskctrl unmount命令时会失败，但是执行umount却能成功。固失败之后补加一个umount流程。
    if (!bRet) {
        iCount = 5;
        strMountPt = XSys::DiskUtil::MountPoint(strPartionName);

        if (!strMountPt.isEmpty()) {
            do {
                XSys::Result result = XSys::SynExec("umount", strPartionName);

                if (!result.isSuccess()) {
                    bRet = false;
                }

                strMountPt = XSys::DiskUtil::MountPoint(strPartionName);

                if (strMountPt.isEmpty()) {
                    bRet = true;
                    break;
                }
                QThread::sleep(3);
            } while (--iCount > 0);
        }
        else {
            bRet = true;
        }
    }

    return bRet;
}

bool UmountDisk(const QString &targetDev)
{
    bool bRet = false;
    int iIndex = targetDev.lastIndexOf("/");

    if (iIndex < 0) {
        return false;
    }

    QString strPath = targetDev.left(iIndex + 1);
    QString strKey = targetDev.right(targetDev.length() - iIndex - 1);

    if (strKey.isEmpty()) {
        return  false;
    }

    QStringList strPartions = GetPartionOfDisk(strKey);

    foreach (QString strPartion, strPartions) {
        QString strVal = strPath + strPartion;
        bRet = UmountPartion(strVal);

        if (!bRet) {
            break;
        }
    }

    return bRet;
}

QString MountPoint(const QString &targetDev)
{
    return XAPI::MountPoint(targetDev);
}

bool Mount(const QString &targetDev, const QString &path)
{
    return XAPI::Mount(targetDev, path);
}

bool Mount(const QString &targetDev)
{
#ifdef Q_OS_LINUX
    return XSys::SynExec("udisksctl", QString("mount -b %1").arg(targetDev)).isSuccess();
#endif
    return true;
}

}

}
