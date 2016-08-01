/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "DiskUtil.h"

#include "../FileSystem/FileSystem.h"
#include "../Cmd/Cmd.h"
#include "Syslinux.h"

#include <QtCore>
#include <QString>


#ifdef Q_OS_WIN32
#include <windows.h>
#include <shellapi.h>
#endif

namespace XAPI {

#ifdef Q_OS_WIN32


BOOL GetStorageDeviceNumberByHandle(HANDLE handle,
                                    const STORAGE_DEVICE_NUMBER* sdn) {
    BOOL result = FALSE;
    DWORD count;

    if(DeviceIoControl(handle, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0,
                       (LPVOID)sdn, sizeof(*sdn), &count, NULL)) {
        result = TRUE;

    } else {
        qWarning("GetDriveNumber: DeviceIoControl failed");
    }

    return (result);
}

int GetPartitionDiskNum(QString targetDev) {
    QString driverName = "\\\\.\\" + targetDev.remove('\\');
    WCHAR wdriverName[1024] = { 0 };
    driverName.toWCharArray(wdriverName);
    HANDLE handle = CreateFile(wdriverName, GENERIC_READ | GENERIC_WRITE,
                               FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                               OPEN_EXISTING, 0, NULL);

    if(handle == INVALID_HANDLE_VALUE) {
        qWarning() << "Open Dev Failed: " << driverName << endl;
        return -1;
    }

    STORAGE_DEVICE_NUMBER sdn;

    if(GetStorageDeviceNumberByHandle(handle, &sdn)) {
        CloseHandle(handle);
        return sdn.DeviceNumber;
    }

    CloseHandle(handle);
    return -1;
}

QString GetPartitionLabel(const QString &targetDev) {
    QString devname = QString("%1:\\").arg(targetDev.at(0));
    WCHAR wLabel[1024] = { 0 };
    if (!GetVolumeInformation(LPWSTR(devname.utf16()), wLabel, 1023, NULL,NULL,NULL,NULL,0) ) {
        return "";
    }
    return QString("").fromWCharArray(wLabel);
}

qint64 GetPartitionFreeSpace(const QString &targetDev) {
    QString devname = QString("%1:\\").arg(targetDev.at(0));
    ULARGE_INTEGER FreeAv,TotalBytes,FreeBytes;
    if  (!GetDiskFreeSpaceEx(LPWSTR(devname.utf16()),&FreeAv,&TotalBytes,&FreeBytes)) {
        return 0;
    }
    return FreeBytes.QuadPart;
}

BOOL GetPartitionByHandle(HANDLE handle, PARTITION_INFORMATION& partInfo){
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
       qWarning()<<"IOCTL_DISK_GET_PARTITION_INFO Failed"<<GetLastError ();
   }
   return result;
}


/*
   return physic driver name like "\\\\.\\PHYSICALDRIVE1";
   */
QString GetPartitionDisk(const QString& targetDev) {
    QString physicalDevName;
    int deviceNum = GetPartitionDiskNum(targetDev);

    if(-1 != deviceNum) {
        physicalDevName = QString("\\\\.\\PHYSICALDRIVE%1").arg(deviceNum);
    }

    return physicalDevName;
}

HANDLE LockDisk(const QString& targetDev) {
    QString phyName = GetPartitionDisk(targetDev);
    WCHAR wPhyName[1024] = { 0 };
    phyName.toWCharArray(wPhyName);
    HANDLE handle = CreateFile(wPhyName, GENERIC_READ | GENERIC_WRITE,
                               FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

    if(handle == INVALID_HANDLE_VALUE)
        return INVALID_HANDLE_VALUE;

    if(!DeviceIoControl(handle, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, NULL, 0)) {
        CloseHandle(handle);
        return INVALID_HANDLE_VALUE;
    }

    return handle;
}

void UnlockDisk(HANDLE handle) {
    DeviceIoControl(handle, IOCTL_DISK_UPDATE_PROPERTIES, NULL, 0, NULL, 0, NULL,
                    0);
    DeviceIoControl(handle, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, NULL, 0);
    CloseHandle(handle);
}

XSys::Result InstallSyslinux(const QString& targetDev) {
    // install syslinux
    XSys::SynExec("label", QString("%1:DEEPINOS").arg(targetDev[0]));
    QString sysliuxPath = XSys::FS::InsertTmpFile(QString(":blobs/syslinux/win32/syslinux.exe"));
    return XSys::SynExec(sysliuxPath, QString(" -i -m -a %1").arg(targetDev));
}

XSys::Result InstallBootloader(const QString& targetDev) {
    qDebug() << "FixUsbDisk Begin!";
    int deviceNum = GetPartitionDiskNum(targetDev);
    QString xfbinstDiskName = QString("(hd%1)").arg(deviceNum);

    // HANDLE handle = LockDisk(targetDev);
    // fbinst format
    QString xfbinstPath = XSys::FS::InsertTmpFile(QString(":blobs/xfbinst/xfbinst.exe"));
    XSys::Result ret= XSys::SynExec(xfbinstPath, QString(" %1 format --fat32 --align --force")
                  .arg(xfbinstDiskName));
    if (!ret.isSuccess()) return ret;

    // install fg.cfg
    QString tmpfgcfgPath = XSys::FS::InsertTmpFile(QString(":blobs/xfbinst/fb.cfg"));
    XSys::SynExec(xfbinstPath, QString(" %1 add-menu fb.cfg \"%2\" ")
                  .arg(xfbinstDiskName)
                  .arg(tmpfgcfgPath));

    XSys::SynExec("label", QString("%1:DEEPINOS").arg(targetDev[0]));

    // install syslinux
    QString sysliuxPath = XSys::FS::InsertTmpFile(QString(":blobs/syslinux/win32/syslinux.exe"));
    XSys::SynExec(sysliuxPath, QString(" -i %1").arg(targetDev));

    // get pbr file ldlinux.bin
    qDebug() << "dump pbr begin";
    QString tmpPbrPath = XSys::FS::TmpFilePath("ldlinux.bin");
    QFile pbr(tmpPbrPath);
    pbr.open(QIODevice::WriteOnly);

    QString targetPhyName = "\\\\.\\" + targetDev;
    QFile targetPhy(targetPhyName);
    targetPhy.open(QIODevice::ReadOnly);

    pbr.write(targetPhy.read(512));
    targetPhy.close();
    pbr.close();
    qDebug() << "dump pbr end";

    // add pbr file ldlinux.bin
    XSys::SynExec(xfbinstPath, QString(" %1 add ldlinux.bin \"%2\" -s")
                  .arg(xfbinstDiskName)
                  .arg(tmpPbrPath));


    // UnlockDisk(handle);
    return XSys::Result(XSys::Result::Success, "", targetDev);
}

XSys::Result UmountDisk(const QString& targetDev) {
    qDebug() << "In Win32 platform, Not UmountDisk " << targetDev;
    return XSys::Result(XSys::Result::Success, "");
}

bool CheckFormatFat32(const QString& targetDev) {
    XSys::Result result = XSys::SynExec( "cmd",  QString("/C \"chcp 437 & fsutil fsinfo volumeinfo %1\" ").arg(targetDev));

    if(result.result().contains("File System Name : FAT32", Qt::CaseInsensitive)) {
        return true;
    }

    return false;
}

const QString MountPoint(const QString& targetDev) {
    return QString(targetDev).remove("/").remove("\\") + "/";
}

bool Mount(const QString& targetDev, const QString& path) {
    return true;
}

#endif

#ifdef Q_OS_UNIX
const QString MountPoint(const QString& targetDev) {
    /*
    Filesystem    512-blocks     Used Available Capacity iused     ifree %iused
    Mounted on
    /dev/disk2s1     3920616  2683872   1236744    69%       0         0  100%
    /Volumes/DEEPINOS 1
    */
    XSys::Result ret = XSys::SynExec("df", "");
    if(!ret.isSuccess())
        return "";
    QStringList mounts = ret.result().split("\n").filter(targetDev);
    if(0 == mounts.size()) {
        return "";
    }
    QString mountinfo = mounts.last();
    mountinfo.remove(targetDev);
    return mountinfo.mid(mountinfo.indexOf('/'));
}


bool Mount(const QString& targetDev, const QString& path) {
    XSys::SynExec("mkdir", QString(" -p %1").arg(path));
    XSys::Result ret = XSys::SynExec("mount", QString(" %1 %2").arg(targetDev).arg(path));
    return ret.isSuccess();
}

#endif

#ifdef Q_OS_LINUX

QString GetPartitionDisk(QString targetDev) {
    if(targetDev.contains(QRegExp("p\\d$")))
        return QString(targetDev).remove(QRegExp("p\\d$"));
    else
        return QString(targetDev).remove(QRegExp("\\d$"));
}

XSys::Result UmountDisk(const QString& targetDev) {
    return XSys::SynExec("bash", QString("-c \"umount -v -f %1?*\"").arg(GetPartitionDisk(targetDev)));
}

bool CheckFormatFat32(const QString& targetDev) {
    XSys::Result ret = XSys::SynExec("blkid", "-s TYPE " + targetDev);
    if(ret.isSuccess() && ret.result().contains("vfat", Qt::CaseInsensitive)) {
        return true;
    }
    return false;
}

QString GetPartitionLabel(const QString &targetDev) {
    XSys::Result ret = XSys::SynExec("blkid", "-s LABEL -o value " + targetDev);
    if (!ret.isSuccess()) {
        return 0;
    }
    return ret.result();
}

qint64 GetPartitionFreeSpace(const QString &targetDev) {
    XSys::Result ret = XSys::SynExec("df", "--output=avail " + targetDev);
    if (!ret.isSuccess()) {
        return 0;
    }
    return ret.result().split("\r").last().remove("\n").toLongLong();
}

XSys::Result InstallSyslinux(const QString& targetDev) {
    // install syslinux
    QString rawtargetDev = GetPartitionDisk(targetDev);
    XSys::Result ret = XSys::Syslinux::InstallBootloader(targetDev);
    if(!ret.isSuccess()) return ret;
    ret = XSys::Syslinux::InstallMbr(rawtargetDev);
    if(!ret.isSuccess()) return ret;

    // make active
    ret = XSys::SynExec(XSys::FS::SearchBin("sfdisk"),
                                     QString("%1 -A %2").arg(rawtargetDev, QString(targetDev).remove(rawtargetDev).remove("p")));
    if(!ret.isSuccess()) return ret;

    // rename label
    ret = XSys::SynExec(XSys::FS::SearchBin("fatlabel"), QString(" %1 DEEPINOS").arg(targetDev));

    return ret;
}

XSys::Result InstallBootloader(const QString& diskDev) {
    XSys::Result ret = UmountDisk(diskDev);

    // pre format
    QString newTargetDev = diskDev + "1";
    QString xfbinstDiskName = QString("\"(hd%1)\"").arg(diskDev[diskDev.length() - 1].toLatin1() - 'a');

    // fbinst: format
    UmountDisk(diskDev);
    QString xfbinstPath = XSys::FS::InsertTmpFile(":blobs/xfbinst/xfbinst");
    ret = XSys::SynExec("chmod", " +x " + xfbinstPath);
    if(!ret.isSuccess()) return ret;

    ret = XSys::SynExec(xfbinstPath, QString(" %1 format --fat32 --align --force").arg(xfbinstDiskName));
    if(!ret.isSuccess()) return ret;

    // fbinst: install fg.cfg
    QString tmpfgcfgPath = XSys::FS::InsertTmpFile(QString(":blobs/xfbinst/fb.cfg"));
    UmountDisk(diskDev);
    ret = XSys::SynExec(xfbinstPath, QString(" %1 add-menu fb.cfg %2 ").arg(xfbinstDiskName).arg(tmpfgcfgPath));
    if(!ret.isSuccess()) return ret;

    // after format, diskdev change to /dev/sd?1
    UmountDisk(diskDev);
    ret = XSys::SynExec("partprobe", QString(" %1").arg(diskDev));
    if(!ret.isSuccess()) return ret;

    // rename label
    ret = XSys::SynExec(XSys::FS::SearchBin("fatlabel"), QString(" %1 DEEPINOS").arg(newTargetDev));
    if(!ret.isSuccess()) return ret;

    // install syslinux
    XSys::Syslinux::InstallBootloader(newTargetDev);

    // dd pbr file ldlinux.bin
    QString tmpPbrPath = XSys::FS::TmpFilePath("ldlinux.bin");
    ret = XSys::SynExec("dd", QString(" if=%1 of=%2 bs=512 count=1").arg(newTargetDev).arg(tmpPbrPath));
    if(!ret.isSuccess()) return ret;

    // fbinst: add pbr file ldlinux.bin
    ret = UmountDisk(diskDev);
    ret = XSys::SynExec(xfbinstPath, QString(" %1 add ldlinux.bin %2 -s").arg(xfbinstDiskName).arg(tmpPbrPath));
    if(!ret.isSuccess()) return ret;

    // mount
    QString mountPoint = QString("/tmp/%1").arg(XSys::FS::TmpFilePath(""));
    ret = XSys::SynExec("mkdir", QString(" -p %1").arg(mountPoint));
    if(!ret.isSuccess()) return ret;

    ret = XSys::SynExec("chmod",  " a+wrx " + mountPoint);
    if(!ret.isSuccess()) return ret;

    QString mountCmd = "-o "
                       "flush,rw,nosuid,nodev,shortname=mixed,"
                       "dmask=0077,utf8=1,showexec %1 %2";
    // the disk must be mount
    int retryTimes = 5;

    do {
        qDebug() << "Try mount the disk " << (6 - retryTimes) << " first time";
        UmountDisk(diskDev);
        QThread::sleep(5);
        XSys::SynExec("partprobe", QString(" %1").arg(diskDev));
        QThread::sleep(5);
        XSys::SynExec("mount",  mountCmd.arg(newTargetDev).arg(mountPoint));
        QThread::sleep(5);
        retryTimes--;
    } while((MountPoint(newTargetDev) == "") && retryTimes);
    // how ever, if mount failed, check before install.
    return XSys::Result(XSys::Result::Success, "", newTargetDev);
}

#endif


#ifdef Q_OS_MAC
QString removAroundSpace(QString raw) {
    QString out;
    for (int i =0; i < raw.length(); ++i) {
        if (' ' != raw.at(i)) {
            out = raw.right(raw.length()-i);
            break;
        }
    }
    raw = out;
    for (int i =0; i < raw.length(); ++i) {
        if (' ' != raw.at(raw.length()-i)) {
            out = raw.left(raw.length()-i);
            break;
        }
    }
    return out;
}


QString GetPartitionLabel(const QString &targetDev) {
    XSys::Result ret = XSys::SynExec("diskutil",  "info " + targetDev);
    if (!ret.isSuccess()) {
        qDebug()<<"Call df Failed";
        return "";
    }
    QString r = ret.result().split("\n").filter("Volume Name:").first().remove("Volume Name:");

    return removAroundSpace(r);
}

qint64 GetPartitionFreeSpace(const QString &targetDev) {
    XSys::Result ret = XSys::SynExec("df", "-b");
    if (!ret.isSuccess()) {
        qDebug()<<"Call df Failed";
        return 0;
    }
    return ret.result().split("\n").filter(targetDev).first().split(" ").filter(QRegExp("[^\\s]")).at(3).toLongLong()*512;
}

QString GetPartitionDisk(QString targetDev) {
    return QString(targetDev).remove(QRegExp("s\\d$"));
}

XSys::Result UmountDisk(const QString& targetDev) {
    return XSys::SynExec("diskutil", QString("unmountDisk force %1").arg(GetPartitionDisk(targetDev)));
}

bool CheckFormatFat32(const QString& targetDev) {
    XSys::Result ret = XSys::SynExec("diskutil", "info " + targetDev);
    QString partitionType = ret.result().split("\n").filter("Partition Type:").first();

    if(partitionType.contains(QRegExp("_FAT_32"))) {
        return true;
    }

    return false;
}


QString Resource(const QString& name) {
    QDir res = QDir(QCoreApplication::applicationDirPath());
    res.cdUp();
    res.cd("Resources");
    return res.absoluteFilePath(name);
}

XSys::Result InstallSyslinux(const QString& targetDev) {
    // rename to DEEPINOS
    XSys::SynExec("diskutil", QString("rename %1 DEEPINOS").arg(targetDev));

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
    return XSys::SynExec("diskutil", QString("mount %1").arg(targetDev));
}

XSys::Result InstallBootloader(const QString& diskDev) {
    QString targetDev = diskDev + "s1";
    QString xfbinstDiskName = QString("\"(hd%1)\"").arg(diskDev[diskDev.length() - 1]);
    // format with xfbinst
    QString xfbinstPath = Resource("xfbinst");

    UmountDisk(targetDev);
    XSys::SynExec(xfbinstPath, QString(" %1 format --fat32 --align --force").arg(xfbinstDiskName));

    // install fg.cfg
    QString tmpfgcfgPath = XSys::FS::InsertTmpFile(QString(":blobs/xfbinst/fb.cfg"));
    UmountDisk(targetDev);
    XSys::SynExec(xfbinstPath, QString(" %1 add-menu fb.cfg %2 ").arg(xfbinstDiskName).arg(tmpfgcfgPath));


    // rename to DEEPINOS
    XSys::SynExec("diskutil", QString("mountDisk %1").arg(diskDev));
    XSys::SynExec("diskutil", QString("rename %1 DEEPINOS").arg(targetDev));

    // install syslinux
    UmountDisk(targetDev);

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

PartionFormat GetPartitionFormat(const QString& targetDev) {
    if(XAPI::CheckFormatFat32(targetDev)) {
        return PF_FAT32;
    }
    return PF_RAW;
}

QString GetPartitionLabel(const QString &targetDev) {
    return XAPI::GetPartitionLabel(targetDev);
}

qint64 GetPartitionFreeSpace(const QString &targetDev) {
    return XAPI::GetPartitionFreeSpace(targetDev);
}

QString GetPartitionDisk(const QString& targetDev) {
    return XAPI::GetPartitionDisk(targetDev);
}

bool EjectDisk(const QString& targetDev) {
    return UmountDisk(GetPartitionDisk(targetDev));
}

bool UmountDisk(const QString& disk) {
    return XAPI::UmountDisk(disk).isSuccess();
}

QString MountPoint(const QString& targetDev) {
    return XAPI::MountPoint(targetDev);
}

bool Mount(const QString& targetDev, const QString& path) {
    return XAPI::Mount(targetDev, path);
}
}

namespace Bootloader {

Result InstallBootloader(const QString& diskDev) {
    return XAPI::InstallBootloader(diskDev);
}

namespace Syslinux {

Result InstallSyslinux(const QString& diskDev) {
     return XAPI::InstallSyslinux(diskDev);
}

Result ConfigSyslinx(const QString& targetPath) {
    // rename isolinux to syslinux
    QString syslinxDir = QString("%1/syslinux/").arg(targetPath);
    if (!XSys::FS::RmDir(syslinxDir)) {
        return Result(Result::Faiiled, "Remove Dir Failed: " + syslinxDir);
    }

    QString isolinxDir = QString("%1/isolinux/").arg(targetPath);
    if (!XSys::FS::MoveDir(isolinxDir, syslinxDir)) {
        return Result(Result::Faiiled, "Move Dir Failed: " + isolinxDir + " to " + syslinxDir);
    }
    qDebug() << "Move " << isolinxDir << " ot " << syslinxDir;

    QString syslinxCfgPath = QString("%1/syslinux/syslinux.cfg").arg(targetPath);
    if (!XSys::FS::RmFile(syslinxCfgPath)) {
        return Result(Result::Faiiled, "Remove File Failed: " + syslinxCfgPath);
    }

    QString isolinxCfgPath = QString("%1/syslinux/isolinux.cfg").arg(targetPath);
    qDebug() << "Rename " << isolinxCfgPath << " ot " << syslinxCfgPath;

    if (!XSys::FS::CpFile(isolinxCfgPath, syslinxCfgPath)) {
        return Result(Result::Faiiled, "Copy File Failed: " + isolinxCfgPath + " to " + syslinxCfgPath);
    }

    qDebug() << "InstallModule to"<<syslinxDir;
    XSys::Syslinux::InstallModule(syslinxDir);

    // bugfix
    // TODO: we change syslinux to 6.02, but gfxboot will not work
    // so use a syslinux.cfg will not use gfxboot and vesamenu
    if (!XSys::FS::InsertFile(":blobs/syslinux/syslinux.cfg", QDir::toNativeSeparators(syslinxDir + "syslinux.cfg"))) {
        return Result(Result::Faiiled, "Insert Config File Failed: :blobs/syslinux/syslinux.cfg to " + QDir::toNativeSeparators(syslinxDir + "syslinux.cfg"));
    }

    return Result(Result::Success, "");
}
}
}
}
