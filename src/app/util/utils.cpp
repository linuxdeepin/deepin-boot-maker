/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "utils.h"

#include <XSys>

#include <QtCore>
#include <QString>


#ifdef Q_OS_WIN32
#include <Windows.h>
#include <QFontDatabase>
#include <QApplication>
#endif


namespace Utils
{

#ifdef Q_OS_WIN32
void loadFonts() {
    QFontDatabase database;
    QStringList fontlist = database.families();

    QStringList preferList;
    preferList.append("Microsoft YaHei");
    preferList.append("微软雅黑");
    preferList.append("SimHei");
    preferList.append("黑体");

    foreach (QString font, preferList) {
        if (fontlist.contains(font)) {
            QFont newFont = QFont(font);
            QApplication::setFont(newFont);
            return;
        }
    }
}
#endif


QString UsbShowText(const QString &dev)
{
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

void ClearTargetDev(const QString &targetPath)
{
    QStringList dirlist;
    dirlist.append("/boot/");
    dirlist.append("/EFI/");
    dirlist.append("/casper/");
    dirlist.append("/live/");
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

#ifdef Q_OS_UNIX
QMap<QString, DeviceInfo> CommandDfParse()
{
    XSys::Result ret = XSys::SynExec("bash", " -c \"df -k --output=source,used,avail\"");
    QString dfout = ret.result();
    QMap<QString, DeviceInfo> deviceInfos;
    foreach(const QString infoline, dfout.split("\n")) {
        QStringList infos = infoline.simplified().split(" ");
        if (infos.size() != 3) {
            continue;
        }
        DeviceInfo devInfo;
        devInfo.path = infos.at(0);
        if (!devInfo.path.contains("/dev/")) {
            continue;
        }
        devInfo.used = static_cast<quint32>(infos.at(1).toInt() / 1024);
        devInfo.total = static_cast<quint32>((infos.at(2).toInt() + infos.at(1).toInt()) / 1024) ;
//        devInfo.target = infos.at(3);
        deviceInfos.insert(devInfo.path, devInfo);
//        qDebug() <<  devInfo.device << devInfo.used << devInfo.total << devInfo.target ;
    }
    return deviceInfos;
}

QMap<QString, DeviceInfo> CommandLsblkParse()
{
    XSys::Result ret = XSys::SynExec("bash", " -c \"lsblk -b -p -J -o name,mountpoint,label,size,uuid\"");
    QString dfout = ret.result();
    QMap<QString, DeviceInfo> deviceInfos;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(dfout.toLatin1());
    foreach(const QJsonValue & value, jsonDoc.object()["blockdevices"].toArray()) {
        foreach(const QJsonValue & partiotion, value.toObject()["children"].toArray()) {
            DeviceInfo info;
            info.path = partiotion.toObject()["name"].toString();
            info.uuid = partiotion.toObject()["uuid"].toString();
            info.label = partiotion.toObject()["label"].toString();
            deviceInfos.insert(info.path, info);
        }
    }
    return deviceInfos;
}
#endif


bool CheckInstallDisk(const QString &targetDev)
{
    if (XSys::DiskUtil::PF_FAT32 != XSys::DiskUtil::GetPartitionFormat(targetDev)) {
        qDebug() << "disk format error " << targetDev;
        return false;
    }

    QString targetPath = XSys::DiskUtil::MountPoint(targetDev);
    qDebug() << "targetPath: " << targetPath;
    QFile test(QDir::toNativeSeparators(targetPath + "/" + "deepinos"));

    if (!test.open(QIODevice::ReadWrite)) {
        qDebug() << "erro open file: " << test.fileName();
        return false;
    }

    QFile deepinos(":src/deepinos");
    deepinos.open(QIODevice::ReadOnly);
    QByteArray data = deepinos.readAll();

    if (data.length() != test.write(data)) {
        qDebug() << "erro write file: " << deepinos.fileName();
        return false;
    }

    test.close();
    deepinos.close();
    test.remove();

    return true;
}

bool isUsbDisk(const QString &dev)
{
    QString out = XSys::FS::TmpFilePath("diskutil_isusb_out");
    XSys::SynExec("bash", QString("-c \" diskutil info %1 > \"%2\" \" ").arg(dev).arg(out));
    QFile outfile(out);
    outfile.open(QIODevice::ReadOnly);
    QString info = outfile.readAll();
    outfile.close();
    outfile.remove();
    return info.contains(QRegExp("Protocol:\\s+USB"));
}

QList<DeviceInfo> ListUsbDrives()
{
    QList<DeviceInfo> deviceList;
#ifdef Q_OS_WIN32
    QFileInfoList extdrivesList = QDir::drives();

    for (int i = 0; i < extdrivesList.size(); ++i) {
        QString deviceLetter = extdrivesList.at(i).path().toUpper();
        if (QDir::toNativeSeparators(deviceLetter) != QDir::toNativeSeparators(QDir::rootPath().toUpper()) && !QDir::toNativeSeparators(deviceLetter)
                .contains("A:") && !QDir::toNativeSeparators(deviceLetter)
                .contains("B:")) {
//            qDebug() << GetDriveType(LPWSTR(deviceLetter.utf16())) << deviceLetter;
            if (GetDriveType(LPWSTR(deviceLetter.utf16())) == 2) {

                DeviceInfo info;
                info.path = QDir::toNativeSeparators(deviceLetter);
                deviceList.push_back(info);
            }
        }
    }
#endif

#ifdef Q_OS_LINUX
    QDir devlstdir("/dev/disk/by-id/");
    QFileInfoList usbfileinfoL = devlstdir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
    QMap<QString, DeviceInfo> dfDeviceInfos = CommandDfParse();
    QMap<QString, DeviceInfo> lsblkDeviceInfos = CommandLsblkParse();

    for (int i = 0; i < usbfileinfoL.size(); ++i) {
        if (usbfileinfoL.at(i).fileName().contains(QRegExp("^usb-\\S{1,}$")) || usbfileinfoL.at(i).fileName().contains(QRegExp("^mmc-\\S{1,}$"))) {
            QString path = usbfileinfoL.at(i).canonicalFilePath();
//            qDebug() << path<<lsblkDeviceInfos.contains(path);
            if (!lsblkDeviceInfos.contains(path)) {
                continue;
            }
            DeviceInfo info = lsblkDeviceInfos.value(path);
            DeviceInfo dfinfo = dfDeviceInfos.value(path, info);
            info.used = dfinfo.used;
            info.total = dfinfo.total;
            info.target = dfinfo.target;
            deviceList.push_back(info);
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

    for (int i = 0; i < usbdevsL.size(); ++i) {
        if (isUsbDisk("/dev/" + usbdevsL.at(i))) {
            fulldrivelist.append("/dev/" + usbdevsL.at(i));
        }
    }

    outfile.close();
    outfile.remove();
#endif
    return deviceList;
}

}