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

bool InstallSyslinux(const QString &targetDev) {
    //install syslinux
    QString sysliuxPath = XSys::InsertTmpFile(QString(":/bootloader/syslinux/syslinux.exe"));
    XSys::SynExec(sysliuxPath , QString(" -i -m -a %1").arg(targetDev));
    return true;
}

bool InstallBootloader(const QString &targetDev) {
    qDebug()<<"FixUsbDisk Begin!";
    int deviceNum = GetPartitionDiskNum(targetDev);
    QString xfbinstDiskName = QString("(hd%1)").arg(deviceNum);

    //HANDLE handle = LockDisk(targetDev);
    //fbinst format
    QString xfbinstPath = XSys::InsertTmpFile(QString(":/bootloader/xfbinst/xfbinst.exe"));
    XSys::SynExec(xfbinstPath, QString(" %1 format --fat32 --align --force").arg(xfbinstDiskName));

    //install fg.cfg
    QString tmpfgcfgPath = XSys::InsertTmpFile(QString(":/bootloader/xfbinst/fb.cfg"));
    XSys::SynExec(xfbinstPath, QString(" %1 add-menu fb.cfg \"%2\" ").arg(xfbinstDiskName).arg(tmpfgcfgPath));

    //install syslinux
    QString sysliuxPath = XSys::InsertTmpFile(QString(":/bootloader/syslinux/syslinux.exe"));
    XSys::SynExec(sysliuxPath , QString(" -i %1").arg(targetDev));

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
    XSys::SynExec(xfbinstPath, QString(" %1 add ldlinux.bin \"%2\" -s").arg(xfbinstDiskName).arg(tmpPbrPath));

    XSys::SynExec("label", QString("%1:DEEPINOS").arg(targetDev[0]));

    //UnlockDisk(handle);
    return true;
}

bool UmountDisk(const QString &targetDev) {
    qDebug()<<"In Win32 platform, Not UmountDisk "<< targetDev;
    return true;
}

bool CheckFormatFat32(const QString &targetDev) {
    QString ret = XSys::SynExec("fsutil",  " fsinfo volumeinfo " + targetDev);
    if (ret.contains("File System Name : FAT32", Qt::CaseInsensitive)) {
        return true;
    }
    return false;
}


const QString MountPoint(const QString &targetDev){
    return QString(targetDev).remove("/").remove("\\") + "/";
}

#endif


#ifdef Q_OS_UNIX
const QString MountPoint(const QString &targetDev){
    /*
Filesystem    512-blocks     Used Available Capacity iused     ifree %iused  Mounted on
/dev/disk2s1     3920616  2683872   1236744    69%       0         0  100%   /Volumes/DEEPINOS 1
    */
    QString ret = XSys::SynExec("df", "");
    QStringList mounts = ret.split("\n").filter(targetDev);
    QString mountinfo = mounts.last();
    mountinfo.remove(targetDev);
    return mountinfo.mid(mountinfo.indexOf('/'));
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

bool CheckFormatFat32(const QString &targetDev) {
    QString ret = XSys::SynExec("blkid -s TYPE ",  targetDev);
    if (ret.contains("vfat", Qt::CaseInsensitive)) {
        return true;
    }
    return false;
}

bool InstallSyslinux(const QString &targetDev) {
    //install syslinux
    //UmountDisk(targetDev);
    QString sysliuxPath = XSys::InsertTmpFile(":/bootloader/syslinux/syslinux");
    XSys::SynExec("chmod +x ", sysliuxPath);
    XSys::SynExec(sysliuxPath , QString(" -i -a %1").arg(targetDev));

    //dd pbr file ldlinux.bin
    QString tmpPbrPath = XSys::InsertTmpFile(":/bootloader/syslinux/mbr.bin");
    XSys::SynExec("dd" , QString(" if=%1 of=%2 ").arg(tmpPbrPath).arg(GetPartitionDisk(targetDev)));

    return true;
}

QString InstallBootloader(const QString &diskDev) {
    XSys::SynExec("bash", QString("-c \"umount -v %1?*\"").arg(diskDev));

    // pre format
    QString newTargetDev = diskDev + "1";
    QString xfbinstDiskName = QString("\"(hd%1)\"").arg(diskDev[diskDev.length() -1].toLatin1() - 'a');

    //fbinst format
    UmountDisk(diskDev);
    QString xfbinstPath = XSys::InsertTmpFile(":/bootloader/xfbinst/xfbinst");
    XSys::SynExec("chmod +x ", xfbinstPath);
    XSys::SynExec(xfbinstPath, QString(" %1 format --fat32 --align --force").arg(xfbinstDiskName));

    //install fg.cfg
    QString tmpfgcfgPath = XSys::InsertTmpFile(QString(":/bootloader/xfbinst/fb.cfg"));
    XSys::SynExec("bash", QString("-c \"umount -v %1?*\"").arg(diskDev));
    XSys::SynExec(xfbinstPath, QString(" %1 add-menu fb.cfg %2 ").arg(xfbinstDiskName).arg(tmpfgcfgPath));

    //install syslinux
    UmountDisk(diskDev);
    QString targetDev = diskDev + "1";
    QString sysliuxPath = XSys::InsertTmpFile(":/bootloader/syslinux/syslinux");
    XSys::SynExec("chmod +x ", sysliuxPath);
    XSys::SynExec(sysliuxPath , QString(" -i %1").arg(targetDev));

    //dd pbr file ldlinux.bin
    QString tmpPbrPath = XSys::TmpFilePath("ldlinux.bin");
    XSys::SynExec("dd" , QString(" if=%1 of=%2 count=1").arg(targetDev).arg(tmpPbrPath));

    //add pbr file ldlinux.bin
    UmountDisk(diskDev);
    XSys::SynExec(xfbinstPath, QString(" %1 add ldlinux.bin %2 -s").arg(xfbinstDiskName).arg(tmpPbrPath));

    //rename label
    XSys::SynExec("fatlabel", QString(" %1 DEEPINOS").arg(newTargetDev));

    //mount
    QString mountPoint =  QString("/tmp/%1").arg(XSys::RandString());
    XSys::SynExec("mkdir", QString(" -p %1").arg(mountPoint));
    XSys::SynExec("chmod a+wrx ", mountPoint);

    QString mountCmd = "mount -o flush,rw,nosuid,nodev,uid=1000,gid=1000,shortname=mixed,dmask=0077,utf8=1,showexec";
    UmountDisk(diskDev);
    XSys::SynExec(mountCmd, QString(" %1 %2").arg(newTargetDev).arg(mountPoint));

    //the disk must be mount
    int retryTimes = 10;
    while ((MountPoint(targetDev) != mountPoint) && retryTimes) {
        UmountDisk(diskDev);
        XSys::SynExec(mountCmd, QString(" %1 %2").arg(newTargetDev).arg(mountPoint));
        QThread::sleep(3);
        retryTimes--;
    }

    return newTargetDev;
}

#endif

#ifdef Q_OS_MAC
QString GetPartitionDisk(QString targetDev) {
    return QString(targetDev).remove(QRegExp("s\\d$"));
}

bool UmountDisk(const QString &targetDev) {
    XSys::SynExec("diskutil", QString("unmountDisk force %1").arg(GetPartitionDisk(targetDev)));
    return true;
}

bool CheckFormatFat32(const QString &targetDev) {
    QString ret = XSys::SynExec("diskutil info ",  targetDev);
    QString partitionType = ret.split("\n").filter("Partition Type:").first();
    if (partitionType.contains(QRegExp("_FAT_32"))) {
        return true;
    }
    return false;
}

bool InstallSyslinux(const QString &targetDev) {
    //install syslinux
    UmountDisk(targetDev);
    QString sysliuxPath = XSys::Resource("syslinux-mac");
    XSys::SynExec(sysliuxPath , QString(" -i %1").arg(targetDev));

    //dd pbr file ldlinux.bin
    UmountDisk(targetDev);
    QString tmpPbrPath = XSys::InsertTmpFile(":/bootloader/syslinux/mbr.bin");
    XSys::SynExec("dd" , QString(" if=%1 of=%2 ").arg(tmpPbrPath).arg(GetPartitionDisk(targetDev)));

    return true;
}

QString InstallBootloader(const QString &diskDev) {
    QString targetDev = diskDev + "s1";
    QString xfbinstDiskName = QString("\"(hd%1)\"").arg(diskDev[diskDev.length() -1]);
    //format with xfbinst
    QString xfbinstPath = XSys::Resource("xfbinst");

    UmountDisk(targetDev);
    XSys::SynExec(xfbinstPath, QString(" %1 format --fat32 --align --force").arg(xfbinstDiskName));

    //install fg.cfg
    QString tmpfgcfgPath = XSys::InsertTmpFile(QString(":/bootloader/xfbinst/fb.cfg"));
    UmountDisk(targetDev);
    XSys::SynExec(xfbinstPath, QString(" %1 add-menu fb.cfg %2 ").arg(xfbinstDiskName).arg(tmpfgcfgPath));

    //install syslinux
    UmountDisk(targetDev);

    QString sysliuxPath = XSys::Resource("syslinux-mac");
    UmountDisk(targetDev);
    XSys::SynExec(sysliuxPath , QString(" -i %1").arg(targetDev));

    //dd pbr file ldlinux.bin
    QString tmpPbrPath = XSys::TmpFilePath("ldlinux.bin");
    UmountDisk(targetDev);
    XSys::SynExec("dd" , QString(" if=%1 of=%2 count=1").arg(targetDev).arg(tmpPbrPath));

    //add pbr file ldlinux.bin
    UmountDisk(targetDev);
    XSys::SynExec(xfbinstPath, QString(" %1 add ldlinux.bin %2 -s").arg(xfbinstDiskName).arg(tmpPbrPath));

    XSys::SynExec("diskutil", QString("mountDisk %1").arg(diskDev));

    //rename to DEEPINOS
    XSys::SynExec("diskutil", QString("rename %1 DEEPINOS").arg(targetDev));

    return targetDev;
}


#endif
}

DiskUnity::DiskUnity(QObject *parent) :
    QObject(parent){
}

QString DiskUnity::InstallBootloader(const QString &diskDev){
    return XAPI::InstallBootloader(diskDev);
}

bool DiskUnity::InstallSyslinux(const QString &diskDev) {
    return XAPI::InstallSyslinux(diskDev);
}

void DiskUnity::ClearTargetDev(const QString &targetPath) {
    QStringList dirlist;
    dirlist.append("/boot/");
    dirlist.append("/EFI/");
    dirlist.append("/casper/");
    dirlist.append("/dists/");
    dirlist.append("/isolinux/");
    dirlist.append("/pool/");
    dirlist.append("/preseed/");
    dirlist.append("/syslinux/");

    foreach(QString dirname, dirlist) {
        QString fullDir = targetPath + dirname;
        XSys::RmDir(fullDir);
    }

#ifdef Q_OS_UNIX
    XSys::SynExec("sync", "");
#endif

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


    QString urlPrifx = ":/bootloader/syslinux/";
#ifdef Q_OS_MAC
    urlPrifx = ":/bootloader/syslinux/macosx/";
#endif

    QStringList filelist;
    filelist.append("gfxboot.c32");
    filelist.append("chain.c32");
    filelist.append("menu.c32");
    filelist.append("vesamenu.c32");
#ifndef Q_OS_MAC
    filelist.append("libcom32.c32");
    filelist.append("libutil.c32");
#endif

    foreach(QString filename, filelist) {
        XSys::InsertFile(urlPrifx + filename, QDir::toNativeSeparators(syslinxDir + filename));
    }

    // bugfix
    // TODO: we change syslinux to 6.02, but gfxboot will not work
    // so use a syslinux.cfg will not use gfxboot and vesamenu
    XSys::InsertFile(":/bootloader/syslinux/syslinux.cfg", QDir::toNativeSeparators(syslinxDir + "syslinux.cfg"));

    return true;
}

bool DiskUnity::CheckInstallDisk(const QString &targetDev) {
    if (!XAPI::CheckFormatFat32(targetDev)) {
        qDebug()<<"disk format error "<<targetDev;
        return false;
    }

    QString targetPath = XAPI::MountPoint(targetDev);
    qDebug()<<"targetPath: "<<targetPath;
    QFile test(QDir::toNativeSeparators(targetPath + "/" + "deepinos"));
    if (!test.open(QIODevice::ReadWrite)) {
        qDebug()<<"erro open file: "<<test.fileName();
        return false;
    }

    QFile deepinos(":/deepinos");
    deepinos.open(QIODevice::ReadOnly);
    QByteArray data = deepinos.readAll();
    if (data.length() != test.write(data)) {
        qDebug()<<"erro write file: "<<deepinos.fileName();
        return false;
    }
    test.close();
    deepinos.close();
    test.remove();

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
    connect(this, SIGNAL(totalSize(qint64, qint64)), SLOT(SetTotalSize(qint64, qint64)));
    connect(this, SIGNAL(toNextFile(const QString &)), SLOT(ToNextFile(const QString &)));
}

void FileListMonitor::ToNextFile(const QString &filename) {
    if (!currentFile_.isEmpty()){
        QFileInfo file(currentFile_);
//        qDebug()<<"file.size() fakesize"<<file.size()<<"\t"<<fakeSizePerFile_;
        finishSize_ += (file.size() + fakeSizePerFile_);
    }
    currentFile_ = filename;
}

void FileListMonitor::SetTotalSize(qint64 size, qint64 total) {
    totalSize_ = size;
    fileNum_ = total;
    fakeSizePerFile_ = totalSize_ /  fileNum_ / 3;
    qDebug()<<"fakesize"<<fakeSizePerFile_;
    totalSize_ = size + fakeSizePerFile_*fileNum_;
    qDebug()<<size<<totalSize_;
}

qint64 FileListMonitor::FinishSize() {
    QFileInfo file(currentFile_);
    return finishSize_ + file.size();
}
