#include "diskunity.h"

#include "xsys.h"

#include <QtCore>

const QString fgcfgData = "timeout 0\ndefault 0\nmenu F2 syslinux \"ldlinux.bin\"\n";

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
QString GetPartitionPhyDevName(QString targetDev) {
    QString physicalDevName;
    int deviceNum = GetPartitionDiskNum(targetDev);
    if (-1 != deviceNum) {
        physicalDevName = QString("\\\\.\\PHYSICALDRIVE%1").arg(deviceNum);
    }
    return physicalDevName;
}

bool DumpPbr(const QString &targetDev, const QString pbrPath) {
    QString driverName = "\\\\.\\" + targetDev.remove('\\');
    WCHAR wdriverName[1024] = {0};
    driverName.toWCharArray(wdriverName);
    HANDLE handle = CreateFile(wdriverName, GENERIC_READ | GENERIC_WRITE,
              FILE_SHARE_READ | FILE_SHARE_WRITE,
              NULL, OPEN_EXISTING, 0, NULL);
    if (handle == INVALID_HANDLE_VALUE) {
        qWarning()<<"Open Dev Failed: "<<driverName<<endl;
        return false;
    }
    return true;
}

bool FormatDisk(const QString &targetDev) {
    qDebug()<<"FixUsbDisk Begin!";
    int deviceNum = GetPartitionDiskNum(targetDev);
    QString diskpartCmd = QString("list disk\r\nselect disk %1\r\nclean\r\ncreate partition primary\r\nlist partition\r\nselect partition 1\r\nformat fs=fat32 label=\"DEEPINOS\" quick\r\nassign letter=%2\r\nactive\r\nlist partition\r\nexit\r\n").arg(deviceNum).arg(targetDev[0]);
    QString cmdfilePath = XSys::InsertTmpFile(diskpartCmd.toLatin1());
    qWarning()<<"FixUsbDisk: cmdfilePath: "<<cmdfilePath;
    XSys::SynExec("diskpart.exe", " /s " + cmdfilePath + " ");
    XSys::RmFile(diskpartTxt);

    QString xfbinstDiskName = QString("(hd%1)").arg(deviceNum);

    //fbinst format
    QString xfbinstPath = XSys::InsertTmpFile(":/xfbinst.exe");
    XSys::SynExec(xfbinstPath, QString(" %1 format --fat32 --align --force").arg(xfbinstDiskName));

    //install fg.cfg
    QString tmpfgcfgPath = XSys::InsertTmpFile(fgcfgData.toLatin1());
    XSys::SynExec(xfbinstPath, QString(" %1 add-menu fg.cfg %2 ").arg(xfbinstDiskName).arg(tmpfgcfgPath));

    //install syslinux
    QString sysliuxPath = XSys::InsertTmpFile(":/syslinux.exe");
    XSys::SynExec(sysliuxPath , QString(" -i -s %1").arg(targetDev));

    //dd pbr file ldlinux.bin
    QString tmpPbrPath = XSys::TmpFilePath("ldlinux.bin");


    XSys::SynExec("dd" , QString(" if=%1 of=%2 count=1").arg(diskDev).arg(tmpPbrPath));

    //add pbr file ldlinux.bin
    XSys::SynExec("bash", QString("-c \"umount -v %1?*\"").arg(diskDev));
    XSys::SynExec(xfbinstPath, QString(" %1 add ldlinux.bin %2 -s").arg(xfbinstDiskName).arg(tmpPbrPath));


    XSys::SynExec("label", QString("%1:DEEPINOS").arg(targetDev[0]));
    return true;
}

bool EjectDisk(const QString &targetDev) {
    return true;
}

#endif

#ifdef Q_OS_LINUX

QString GetPartitionDiskDev(QString targetDev) {
    if (targetDev.contains(QRegExp("p\\d$")))
        return QString(targetDev).remove(QRegExp("p\\d$"));
    else
        return QString(targetDev).remove(QRegExp("\\d$"));
}

QString FormatDisk(const QString &diskDev) {
    XSys::SynExec("bash", QString("-c \"umount -v %1?*\"").arg(diskDev));

    // pre format
    QString newTargetDev = diskDev + "1";
    XSys::SynExec("dd", QString(" if=/dev/zero of=%1 count=1024").arg(diskDev));
    XSys::SynExec("fdisk", QString(" %1 ").arg(diskDev), QString("o\nn\np\n\n\n\na\n1\nt\nb\nw\n"));
    XSys::SynExec("bash", QString("-c \"umount -v %1?*\"").arg(diskDev));
    XSys::SynExec("mkfs.fat -F32 -v -I -n \"DeepinOS\" ", newTargetDev);

    QString xfbinstDiskName = QString("\"(hd%1)\"").arg(diskDev[diskDev.length() -1]);

    //fbinst format
    XSys::SynExec("bash", QString("-c \"umount -v %1?*\"").arg(diskDev));
    QString xfbinstPath = XSys::InsertTmpFile(":/xfbinst");
    XSys::SynExec(xfbinstPath, QString(" %1 format --fat32 --align --force").arg(xfbinstDiskName));

    //install fg.cfg
    QString tmpfgcfgPath = XSys::InsertTmpFile(fgcfgData.toLatin1());
    XSys::SynExec("bash", QString("-c \"umount -v %1?*\"").arg(diskDev));
    XSys::SynExec(xfbinstPath, QString(" %1 add-menu fg.cfg %2 ").arg(xfbinstDiskName).arg(tmpfgcfgPath));

    //install syslinux
    XSys::SynExec("bash", QString("-c \"umount -v %1?*\"").arg(diskDev));
    QString targetDev = diskDev + "s1";
    QString sysliuxPath = XSys::InsertTmpFile(":/ubnsylnx");
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
    QString mountPoint =  QString("/media/%1").arg(XSys::RandString());
    XSys::SynExec("mkdir", QString(" -p %1").arg(mountPoint));
    XSys::SynExec("chmod a+wrx ", mountPoint);

    XSys::SynExec("mount -o flush,rw,nosuid,nodev,uid=1000,gid=1000,shortname=mixed,dmask=0077,utf8=1,showexec", QString(" %1 %2").arg(newTargetDev).arg(mountPoint));
    return newTargetDev;
}

bool EjectDisk(const QString &targetDev) {
    XSys::SynExec("bash", QString("-c \"umount -v %1?*\"").arg(GetPartitionDiskDev(targetDev)));
    return true;
}

#endif

#ifdef Q_OS_MAC
QString GetPartitionDiskDev(QString targetDev) {
    return QString(targetDev).remove(QRegExp("s\\d$"));
}

QString FormatDisk(const QString &diskDev) {
    XSys::SynExec("diskutil", QString("unmountDisk %1").arg(diskDev));

    QString xfbinstDiskName = QString("\"(hd%1)\"").arg(diskDev[diskDev.length() -1]);

    //format with xfbinst
    QString xfbinstPath = XSys::Resource("xfbinst");
    XSys::SynExec(xfbinstPath, QString(" %1 format --fat32 --align --force").arg(xfbinstDiskName));

    //install fg.cfg
    QString tmpfgcfgPath = XSys::InsertTmpFile(fgcfgData.toLatin1());
    XSys::SynExec("diskutil", QString("unmountDisk %1").arg(diskDev));
    XSys::SynExec(xfbinstPath, QString(" %1 add-menu fg.cfg %2 ").arg(xfbinstDiskName).arg(tmpfgcfgPath));

    //install syslinux
    XSys::SynExec("diskutil", QString("unmountDisk %1").arg(diskDev));
    QString targetDev = diskDev + "s1";
    QString sysliuxPath = XSys::Resource("syslinux-mac");
    XSys::SynExec(sysliuxPath , QString(" -i -s %1").arg(targetDev));

    //dd pbr file ldlinux.bin
    QString tmpPbrPath = XSys::TmpFilePath("ldlinux.bin");
    XSys::SynExec("dd" , QString(" if=%1 of=%2 count=1").arg(diskDev).arg(tmpPbrPath));

    //add pbr file ldlinux.bin
    XSys::SynExec("diskutil", QString("unmountDisk %1").arg(diskDev));
    XSys::SynExec(xfbinstPath, QString(" %1 add ldlinux.bin %2 -s").arg(xfbinstDiskName).arg(tmpPbrPath));

    //rename to DEEPINOS
    XSys::SynExec("diskutil", QString("rename %1 DEEPINOS").arg(targetDev));

    XSys::SynExec("diskutil", QString("mountDisk %1").arg(diskDev));
    return targetDev;
}

bool EjectDisk(const QString &targetDev) {
    XSys::SynExec("diskutil", QString("unmountDisk %1").arg(GetPartitionDiskDev(targetDev)));
    return true;
}
#endif
}

DiskUnity::DiskUnity(QObject *parent) :
    QObject(parent){
}

QString DiskUnity::FormatDisk(const QString &diskDev){
    return XAPI::FormatDisk(diskDev);
}

bool DiskUnity::EjectDisk(const QString &targetDev) {
    return XAPI::EjectDisk(targetDev);
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
