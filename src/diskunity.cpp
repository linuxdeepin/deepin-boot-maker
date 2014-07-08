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
QString GetPartitionPhyDevName(QString targetDev) {
    QString physicalDevName;
    int deviceNum = GetPartitionDiskNum(targetDev);
    if (-1 != deviceNum) {
        physicalDevName = QString("\\\\.\\PHYSICALDRIVE%1").arg(deviceNum);
    }
    return physicalDevName;
}

bool FixUsbDisk(QString targetDev) {
    qDebug()<<"FixUsbDisk Begin!";
    int deviceNum = GetPartitionDiskNum(targetDev);
    QString diskpartCmd = QString("list disk\r\nselect disk %1\r\nclean\r\ncreate partition primary\r\nlist partition\r\nselect partition 1\r\nformat fs=fat32 quick\r\nassign letter=%2\r\nactive\r\nlist partition\r\nexit\r\n").arg(deviceNum).arg(targetDev[0]);
    QString cmdfilePath = XSys::TmpFilePath("diskpart.txt");
    qWarning()<<"FixUsbDisk: cmdfilePath: "<<cmdfilePath;
    QFile diskpartTxt(cmdfilePath);
    diskpartTxt.open(QIODevice::WriteOnly);
    diskpartTxt.write(diskpartCmd.toLatin1());
    diskpartTxt.close();

    XSys::SynExec("diskpart.exe", " /s " + cmdfilePath + " ");

    XSys::RmFile(diskpartTxt);
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

int FixUsbDisk(QString targetDev) {
    qDebug()<<"Fix Usb Disk"<<targetDev;
    QString diskDev = GetPartitionDiskDev(targetDev);
    XSys::CpFile(":/mbr.bin", diskDev);
    return true;
}

#endif

#ifdef Q_OS_MAC
int FixUsbDisk(QString targetDev);
QString GetPartitionDiskDev(QString targetDev) {
    return QString(targetDev).remove(QRegExp("s\\d$"));
}
#endif
}

DiskUnity::DiskUnity(QObject *parent) :
    QObject(parent){
}

bool DiskUnity::FixUsbDisk(const QString &targetDev) {
    return XAPI::FixUsbDisk(targetDev);
}

QString DiskUnity::FormatDisk(const QString &diskDev) {
//    for (int i = 1; i < 32; ++i) {
//        XSys::SynExec("umount", (" -v " + diskDev+"%1").arg(i));
//    }
    XSys::SynExec("dd", QString(" if=/dev/zero of=%1 count=1024").arg(diskDev));
    XSys::SynExec("sync", "");
    QString cmdfile = XSys::InsertTmpFile(QString("o\nn\np\n\n\n\n").toLatin1());
    XSys::SynExec("fdisk", QString(" %1 < %2").arg(diskDev).arg(cmdfile));
    QString newTargetDev = diskDev + "1";
    XSys::SynExec("umount", " -v " + newTargetDev);
    XSys::SynExec("mkfs.fat", newTargetDev);
    QString mountPoint =  QString("/tmp/%1").arg(XSys::RandString());
    XSys::SynExec("sync", "");
    XSys::SynExec("mkdir", QString(" -p %1").arg(mountPoint));
    XSys::SynExec("mount", QString(" %1 %2").arg(newTargetDev).arg(mountPoint));
    XSys::SynExec("sync", "");
    return newTargetDev;
}
