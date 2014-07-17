#include "diskunity.h"

#include "xsys.h"

#include <QtCore>

namespace XAPI {
#ifdef Q_OS_WIN32

#include <windows.h>
#include <shellapi.h>

BOOL GetStorageDeviceNumberByHandle(HANDLE handle,
                    const STORAGE_DEVICE_NUMBER * sdn)
{
    BOOL result = FALSE;
    DWORD count;

    if (DeviceIoControl(handle, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL,
            0, (LPVOID) sdn, sizeof(*sdn), &count, NULL)) {
        result = TRUE;
    } else {
        qWarning("GetDriveNumber: DeviceIoControl failed");
    }

    return (result);
}

int GetPartitionDiskNum(QString targetDev) {
    QString driverName = "\\\\.\\" + targetDev.remove('\\');
    WCHAR wdriverName[1024] = {0};
    driverName.toWCharArray(wdriverName);
    HANDLE handle = CreateFile(wdriverName, GENERIC_READ | GENERIC_WRITE,
              FILE_SHARE_READ | FILE_SHARE_WRITE,
              NULL, OPEN_EXISTING, 0, NULL);
    if (handle == INVALID_HANDLE_VALUE) {
        qWarning()<<"Open Dev Failed: "<<driverName<<endl;
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

/*
 return physic driver name like "\\\\.\\PHYSICALDRIVE1";
*/
QString GetPartitionDisk(const QString &targetDev) {
    QString physicalDevName;
    int deviceNum = GetPartitionDiskNum(targetDev);
    if (-1 != deviceNum) {
        physicalDevName = QString("\\\\.\\PHYSICALDRIVE%1").arg(deviceNum);
    }
    return physicalDevName;
}


HANDLE LockDisk (const QString &targetDev) {
    QString phyName = GetPartitionDisk(targetDev);
    WCHAR wPhyName[1024] = {0};
    phyName.toWCharArray(wPhyName);
    HANDLE handle = CreateFile (wPhyName, GENERIC_READ | GENERIC_WRITE,
                        FILE_SHARE_READ | FILE_SHARE_WRITE,
                        NULL, OPEN_EXISTING, 0, 0);
    if (handle == INVALID_HANDLE_VALUE)
        return INVALID_HANDLE_VALUE;

    if (! DeviceIoControl (handle, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, NULL, 0)) {
        CloseHandle(handle);
        return INVALID_HANDLE_VALUE;
    }
    return handle;
}

void UnlockDisk(HANDLE handle) {
    DeviceIoControl (handle, IOCTL_DISK_UPDATE_PROPERTIES, NULL, 0, NULL, 0, NULL, 0);
    DeviceIoControl (handle, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, NULL, 0);
    CloseHandle (handle);
}

bool InstallBootloader(const QString &targetDev) {

    qDebug()<<"FixUsbDisk Begin!";
    int deviceNum = GetPartitionDiskNum(targetDev);
    QString xfbinstDiskName = QString("(hd%1)").arg(deviceNum);

    HANDLE handle = LockDisk(targetDev);
    //fbinst format
    QString xfbinstPath = XSys::InsertTmpFile(QString(":/bootloader/xfbinst/xfbinst.exe"));
    XSys::SynExec(xfbinstPath, QString(" %1 format --fat32 --align --force").arg(xfbinstDiskName));

    //install fg.cfg
    QString tmpfgcfgPath = XSys::InsertTmpFile(QString(":/bootloader/xfbinst/fb.cfg"));
    XSys::SynExec(xfbinstPath, QString(" %1 add-menu fb.cfg \"%2\" ").arg(xfbinstDiskName).arg(tmpfgcfgPath));

    //install syslinux
    QString sysliuxPath = XSys::InsertTmpFile(QString(":/bootloader/syslinux/syslinux.exe"));
    XSys::SynExec(sysliuxPath , QString(" -i -s %1").arg(targetDev));

    //get pbr file ldlinux.bin
    qDebug()<<"dump pbr begin";
    QString tmpPbrPath = XSys::TmpFilePath("ldlinux.bin");
    QFile pbr(tmpPbrPath);
    pbr.open(QIODevice::WriteOnly);

    QString targetPhyName = "\\\\.\\" + targetDev;
    QFile targetPhy(targetPhyName);
    targetPhy.open(QIODevice::ReadOnly);

    pbr.write(targetPhy.read(512));
    targetPhy.close();
    pbr.close();
    qDebug()<<"dump pbr end";

    //add pbr file ldlinux.bin
    XSys::SynExec(xfbinstPath, QString(" %1 add ldlinux.bin %2 -s").arg(xfbinstDiskName).arg(tmpPbrPath));

    XSys::SynExec("label", QString("%1:DEEPINOS").arg(targetDev[0]));

    UnlockDisk(handle);
    return true;
}

bool UmountDisk(const QString &targetDev) {
    return true;
}

#endif

#ifdef Q_OS_LINUX

QString GetPartitionDisk(QString targetDev) {
    if (targetDev.contains(QRegExp("p\\d$")))
        return QString(targetDev).remove(QRegExp("p\\d$"));
    else
        return QString(targetDev).remove(QRegExp("\\d$"));
}

bool UmountDisk(const QString &targetDev) {
    XSys::SynExec("bash", QString("-c \"umount -v %1?*\"").arg(GetPartitionDisk(targetDev)));
    return true;
}

QString InstallBootloader(const QString &diskDev) {
    XSys::SynExec("bash", QString("-c \"umount -v %1?*\"").arg(diskDev));

    // pre format
    QString newTargetDev = diskDev + "1";
    QString xfbinstDiskName = QString("\"(hd%1)\"").arg(diskDev[diskDev.length() -1]);

    //fbinst format
    XSys::SynExec("bash", QString("-c \"umount -v %1?*\"").arg(diskDev));
    QString xfbinstPath = XSys::InsertTmpFile(":/bootloader/xfbinst/xfbinst");
    XSys::SynExec(xfbinstPath, QString(" %1 format --fat32 --align --force").arg(xfbinstDiskName));

    //install fg.cfg
    QString tmpfgcfgPath = XSys::InsertTmpFile(QString(":/bootloader/xfbinst/fb.cfg"));
    XSys::SynExec("bash", QString("-c \"umount -v %1?*\"").arg(diskDev));
    XSys::SynExec(xfbinstPath, QString(" %1 add-menu fb.cfg %2 ").arg(xfbinstDiskName).arg(tmpfgcfgPath));

    //install syslinux
    XSys::SynExec("bash", QString("-c \"umount -v %1?*\"").arg(diskDev));
    QString targetDev = diskDev + "s1";
    QString sysliuxPath = XSys::InsertTmpFile(":/bootloader/xfbinst/syslinux");
    XSys::SynExec(sysliuxPath , QString(" -i -s %1").arg(targetDev));

    //dd pbr file ldlinux.bin
    QString tmpPbrPath = XSys::TmpFilePath("ldlinux.bin");
    XSys::SynExec("dd" , QString(" if=%1 of=%2 count=1").arg(diskDev).arg(tmpPbrPath));

    //add pbr file ldlinux.bin
    XSys::SynExec("bash", QString("-c \"umount -v %1?*\"").arg(diskDev));
    XSys::SynExec(xfbinstPath, QString(" %1 add ldlinux.bin %2 -s").arg(xfbinstDiskName).arg(tmpPbrPath));

    //rename label
    XSys::SynExec("fatlabel", QString(" %1 DEEPINOS").arg(newTargetDev));

    //mount
    QString mountPoint =  QString("/tmp/%1").arg(XSys::RandString());
    XSys::SynExec("mkdir", QString(" -p %1").arg(mountPoint));
    XSys::SynExec("chmod a+wrx ", mountPoint);

    XSys::SynExec("mount -o flush,rw,nosuid,nodev,uid=1000,gid=1000,shortname=mixed,dmask=0077,utf8=1,showexec", QString(" %1 %2").arg(newTargetDev).arg(mountPoint));
    return newTargetDev;
}


#endif

#ifdef Q_OS_MAC
QString GetPartitionDisk(QString targetDev) {
    return QString(targetDev).remove(QRegExp("s\\d$"));
}

QString InstallBootloader(const QString &diskDev) {
    UmountDisk(diskDev);
    QString xfbinstDiskName = QString("\"(hd%1)\"").arg(diskDev[diskDev.length() -1]);

    //format with xfbinst
    QString xfbinstPath = XSys::Resource("xfbinst");
    XSys::SynExec(xfbinstPath, QString(" %1 format --fat32 --align --force").arg(xfbinstDiskName));

    //install fg.cfg
    QString tmpfgcfgPath = XSys::InsertTmpFile(QString(":/bootloader/xfbinst/fb.cfg"));
    UmountDisk(diskDev);
    XSys::SynExec(xfbinstPath, QString(" %1 add-menu fb.cfg %2 ").arg(xfbinstDiskName).arg(tmpfgcfgPath));

    //install syslinux
    UmountDisk(diskDev);
    QString targetDev = diskDev + "s1";
    QString sysliuxPath = XSys::Resource("syslinux-mac");
    XSys::SynExec(sysliuxPath , QString(" -i -s %1").arg(targetDev));

    //dd pbr file ldlinux.bin
    QString tmpPbrPath = XSys::TmpFilePath("ldlinux.bin");
    XSys::SynExec("dd" , QString(" if=%1 of=%2 count=1").arg(diskDev).arg(tmpPbrPath));

    //add pbr file ldlinux.bin
    UmountDisk(diskDev);
    XSys::SynExec(xfbinstPath, QString(" %1 add ldlinux.bin %2 -s").arg(xfbinstDiskName).arg(tmpPbrPath));

    //rename to DEEPINOS
    XSys::SynExec("diskutil", QString("rename %1 DEEPINOS").arg(targetDev));

    XSys::SynExec("diskutil", QString("mountDisk %1").arg(diskDev));
    return targetDev;
}

bool UmountDisk(const QString &targetDev) {
    XSys::SynExec("diskutil", QString("unmountDisk %1").arg(GetPartitionDisk(targetDev)));
    return true;
}
#endif
}

DiskUnity::DiskUnity(QObject *parent) :
    QObject(parent){
}

QString DiskUnity::InstallBootloader(const QString &diskDev){
    return XAPI::InstallBootloader(diskDev);
}

bool DiskUnity::ConfigSyslinx(const QString &targetPath) {
    //rename isolinux to syslinux
    QString syslinxDir = QString("%1syslinux/").arg(targetPath);
    XSys::RmDir(syslinxDir);

    QString isolinxDir = QString("%1isolinux/").arg(targetPath);
    XSys::MoveDir(isolinxDir, syslinxDir);
    qDebug()<<"Rename "<<isolinxDir<<" ot "<<syslinxDir;

    QString syslinxCfgPath = QString("%1syslinux/syslinux.cfg").arg(targetPath);
    XSys::RmFile(syslinxCfgPath);

    QString isolinxCfgPath = QString("%1syslinux/isolinux.cfg").arg(targetPath);
    qDebug()<<"Rename "<<isolinxCfgPath<<" ot "<<syslinxCfgPath;

    XSys::CpFile(isolinxCfgPath, syslinxCfgPath);

    QStringList filelist;
    filelist.append("gfxboot.c32");
    filelist.append("libcom32.c32");
    filelist.append("libutil.c32");
    filelist.append("menu.c32");
    filelist.append("vesamenu.c32");

    foreach(QString filename, filelist) {
        XSys::InsertFile(":/bootloader/syslinux/" + filename, QDir::toNativeSeparators(syslinxDir + filename));
    }
    return true;
}

QString DiskUnity::GetPartitionDisk(const QString &targetDev){
    return XAPI::GetPartitionDisk(targetDev);
}

bool DiskUnity::EjectDisk(const QString &targetDev) {
    return UmountDisk(GetPartitionDisk(targetDev));
}

bool DiskUnity::UmountDisk(const QString &disk) {
    return XAPI::UmountDisk(disk);
}

FileListMonitor::FileListMonitor(QObject *parent) :QObject(parent){
    finishSize_ = 0;
    connect(this, SIGNAL(totalSize(qint64)), SLOT(SetTotalSize(qint64)));
    connect(this, SIGNAL(toNextFile(const QString &)), SLOT(ToNextFile(const QString &)));
}

void FileListMonitor::ToNextFile(const QString &filename) {
    if (!currentFile_.isEmpty()){
        QFileInfo file(currentFile_);
        finishSize_ += file.size();
    }
    currentFile_ = filename;
}

void FileListMonitor::SetTotalSize(qint64 size) {
    totalSize_ = size;
}

qint64 FileListMonitor::FinishSize() {
    QFileInfo file(currentFile_);
    return finishSize_ + file.size();
}
