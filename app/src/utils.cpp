#include "utils.h"

#include <XSys>

#include <QtCore>
#include <QString>

#ifdef Q_OS_WIN32
#include <Windows.h>
#endif
namespace Utils {

QString UsbShowText(const QString &dev) {
    QString showText;
    if (!dev.isEmpty()) {
        QString label = XSys::DiskUtil::GetPartitionLabel(dev);
        if (label.isEmpty()) {
            label = QObject::tr("Removable disk");
        }
#ifdef Q_OS_UNIX
        QFileInfo devFile(dev);
        showText = QString("%1: %2").arg(devFile.baseName()).arg(label);
#endif
#ifdef Q_OS_WIN32
    showText = QString("(%1:) %2").arg(dev.at(0)).arg(label);
#endif
    }
    return showText;
}


void ClearTargetDev(const QString& targetPath) {
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
        XSys::FS::RmDir(fullDir);
    }

#ifdef Q_OS_UNIX
    XSys::SynExec("sync", "");
#endif
}

bool CheckInstallDisk(const QString& targetDev) {
    if(XSys::DiskUtil::PF_FAT32 != XSys::DiskUtil::GetPartitionFormat(targetDev)) {
        qDebug() << "disk format error " << targetDev;
        return false;
    }

    QString targetPath = XSys::DiskUtil::MountPoint(targetDev);
    qDebug() << "targetPath: " << targetPath;
    QFile test(QDir::toNativeSeparators(targetPath + "/" + "deepinos"));

    if(!test.open(QIODevice::ReadWrite)) {
        qDebug() << "erro open file: " << test.fileName();
        return false;
    }

    QFile deepinos(":src/deepinos");
    deepinos.open(QIODevice::ReadOnly);
    QByteArray data = deepinos.readAll();

    if(data.length() != test.write(data)) {
        qDebug() << "erro write file: " << deepinos.fileName();
        return false;
    }

    test.close();
    deepinos.close();
    test.remove();

    return true;
}

bool isUsbDisk(const QString& dev) {
    QString out = XSys::FS::TmpFilePath("diskutil_isusb_out");
    XSys::SynExec(
        "bash", QString("-c \" diskutil info %1 > \"%2\" \" ").arg(dev).arg(out));
    QFile outfile(out);
    outfile.open(QIODevice::ReadOnly);
    QString info = outfile.readAll();
    outfile.close();
    outfile.remove();
    return info.contains(QRegExp("Protocol:\\s+USB"));
}

QStringList ListUsbDrives() {
    QStringList fulldrivelist;

#ifdef Q_OS_WIN32
    QFileInfoList extdrivesList = QDir::drives();

    for(int i = 0; i < extdrivesList.size(); ++i) {
        if(QDir::toNativeSeparators(extdrivesList.at(i).path().toUpper()) != QDir::toNativeSeparators(QDir::rootPath().toUpper()) && !QDir::toNativeSeparators(extdrivesList.at(i).path().toUpper())
                .contains("A:") && !QDir::toNativeSeparators(extdrivesList.at(i).path().toUpper())
                .contains("B:")) {
            if(GetDriveType(LPWSTR(extdrivesList.at(i).path().toUpper().utf16())) == 2) {
                fulldrivelist.append(
                    QDir::toNativeSeparators(extdrivesList.at(i).path().toUpper()));
            }
        }
    }

#endif
#ifdef Q_OS_LINUX
    QDir devlstdir("/dev/disk/by-id/");
    QFileInfoList usbfileinfoL = devlstdir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);

    for(int i = 0; i < usbfileinfoL.size(); ++i) {
        if(usbfileinfoL.at(i).fileName().contains(QRegExp("^usb-\\S{1,}$")) || usbfileinfoL.at(i).fileName().contains(QRegExp("^mmc-\\S{1,}$"))) {
            XSys::Result ret = XSys::SynExec("blkid", QString("-s TYPE %2").arg(usbfileinfoL.at(i).canonicalFilePath()));
            QString tstrblk = ret.result();
            if(tstrblk.contains('=')) {
                if(tstrblk.section('=', -1, -1).remove('"').contains(
                            QRegExp("(vfat|ext2|ext3|ext4)")))
                    fulldrivelist.append(usbfileinfoL.at(i).canonicalFilePath());
            }
        }
    }

#endif
#ifdef Q_OS_MAC
    QString out = XSys::FS::TmpFilePath("diskutil_out");
    XSys::SynExec("bash", QString("-c \" diskutil list > \"%1\" \" ").arg(out));
    QFile outfile(out);
    outfile.open(QIODevice::ReadOnly);
    QString diskutilList = outfile.readAll();
    QStringList usbdevsL = diskutilList.split("\n").filter(QRegExp("(FAT|Microsoft)")).join(" ").split(" ").filter("disk");

    for(int i = 0; i < usbdevsL.size(); ++i) {
        if(isUsbDisk("/dev/" + usbdevsL.at(i))) {
            fulldrivelist.append("/dev/" + usbdevsL.at(i));
        }
    }

    outfile.close();
    outfile.remove();
#endif
    return fulldrivelist;
}

}



FileListMonitor::FileListMonitor(QObject* parent)
    : QObject(parent) {
    finishSize_ = 0;
    connect(this, SIGNAL(totalSize(qint64, qint64)),
            SLOT(SetTotalSize(qint64, qint64)));
    connect(this, SIGNAL(toNextFile(const QString&)),
            SLOT(ToNextFile(const QString&)));
}

void FileListMonitor::ToNextFile(const QString& filename) {
    qDebug() << "Monitor: " << filename;

    if(!currentFile_.isEmpty()) {
        QFileInfo file(currentFile_);
        //        qDebug()<<"file.size()
        // fakesize"<<file.size()<<"\t"<<fakeSizePerFile_;
        finishSize_ += (file.size() + fakeSizePerFile_);
    }

    currentFile_ = filename;
}

void FileListMonitor::SetTotalSize(qint64 size, qint64 total) {
    totalSize_ = size;
    fileNum_ = total;
    fakeSizePerFile_ = totalSize_ / fileNum_ / 3;
    qDebug() << "fakesize" << fakeSizePerFile_;
    totalSize_ = size + fakeSizePerFile_ * fileNum_;
    qDebug() << size << totalSize_;
}

qint64 FileListMonitor::FinishSize() {
    QFileInfo file(currentFile_);
    return finishSize_ + file.size();
}
