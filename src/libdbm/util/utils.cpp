// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "utils.h"

#include <QtCore>
#include <QString>

#include <XSys>
#include <dirent.h>
#include <QTextCodec>
#include <QByteArray>
#include <QRegExp>
#include <qglobal.h>
#ifdef Q_OS_WIN32
#include <Windows.h>
#endif

static void initQRC()
{
#ifdef Q_OS_LINUX
    //Q_INIT_RESOURCE(blob_linux);
#else
    Q_INIT_RESOURCE(blob);
#endif
}

namespace Utils {

bool isUft8(const QByteArray& byArr)
{
    unsigned int nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
    bool bAllAscii = true;

    for (int i = 0;  i < byArr.length(); ++i) {
        unsigned char chr = static_cast<unsigned char>(byArr.at(i));

        //ASCII码高位为0
        if (nBytes == 0 && (chr & 0x80) != 0) {
            bAllAscii = false;
        }

        if (nBytes == 0) {
            //计算连续1的位数
            if (chr >= 0x80) {
                if (chr >= 0xFC && chr <= 0xFD) {
                    nBytes = 6;
                }
                else if (chr >= 0xF8) {
                    nBytes = 5;
                }
                else if (chr >= 0xF0) {
                    nBytes = 4;
                }
                else if (chr >= 0xE0) {
                    nBytes = 3;
                }
                else if (chr >= 0xC0) {
                    nBytes = 2;
                }
                else {
                    return false;
                }

                nBytes--;
            }
        }
        else {
            if ((chr & 0xC0) != 0x80) {
                return false;
            }

            nBytes--;
        }
    }

    if (nBytes != 0)  {
        return false;
    }

    if (bAllAscii) { //如果全部都是ASCII, 也是UTF8
        return true;
    }

    return true;
}

bool isGBK(const QByteArray& byArr)
{
    unsigned int nBytes = 0;
    bool bAllAscii = true;

    for (int i = 0;  i < byArr.length(); ++i) {
        unsigned char chr = static_cast<unsigned char>(byArr.at(i));

        //ASCII码高位为0
        if ((chr & 0x80) != 0 && nBytes == 0) {
            bAllAscii = false;
        }

        if (nBytes == 0) {
            if (chr >= 0x80) {
                if (chr >= 0x81 && chr <= 0xFE) {
                    nBytes = +2;
                }
                else {
                    return false;
                }

                nBytes--;
            }
        }
        else {
            if (chr < 0x40 || chr>0xFE) {
                return false;
            }

            nBytes--;
        }
    }

    if (nBytes != 0) {//违返规则
        return false;
    }

    if (bAllAscii){ //如果全部都是ASCII, 也是GBK
        return true;
    }

    return true;
}

void loadTranslate()
{
    QTranslator *qtTranslator = new QTranslator;
    qtTranslator->load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    qApp->installTranslator(qtTranslator);
    qtTranslator->deleteLater();
    qtTranslator = nullptr;

#ifdef Q_OS_WIN
    QTranslator *translator = new QTranslator();
    QString tnapplang;
    QString tnappcoun;
    QString clangcode = "";
    QStringList allappargs =  qApp->arguments();
    QList<QPair<QString, QString> > oppairs;

    for (QList<QString>::const_iterator i = allappargs.constBegin(); i < allappargs.constEnd(); ++i) {
        if (i->count('=') == 1) {
            oppairs.append(QPair<QString, QString> (i->section('=', 0, 0).simplified(), i->section('=', 1, 1).simplified()));
        }
    }

    for (QList<QPair<QString, QString> >::const_iterator i = oppairs.constBegin(); i < oppairs.constEnd(); ++i) {
        if (i->first.contains("lang", Qt::CaseInsensitive)) {
            clangcode = i->second;
            tnapplang = clangcode.left(2);

            if (clangcode.contains('_') && clangcode.size() == 5) {
                tnappcoun = clangcode.section('_', -1, -1);
            }

            break;
        }
    }

    if (clangcode.isEmpty()) {
        clangcode = QLocale::system().name();
        tnapplang = clangcode.left(2);

        if (clangcode.contains('_') && clangcode.size() == 5) {
            tnappcoun = clangcode.section('_', -1, -1);
        }
    }

    QString tranlateUrl;
    if (!tnappcoun.isEmpty()) {
        tranlateUrl = QString(":/translations/deepin-boot-maker_%1_%2.qm").arg(tnapplang).arg(tnappcoun);
    }

    if (!QFile::exists(tranlateUrl)) {
        tranlateUrl = QString(":/translations/deepin-boot-maker_%1.qm").arg(tnapplang);
    }


    if (!QFile::exists(tranlateUrl)) {
        tranlateUrl = ":/translations/deepin-boot-maker.qm";
    }

    qDebug() << "locate:" << clangcode;
    qDebug() << "load:" << tranlateUrl;

    if (translator->load(tranlateUrl)) {
        qApp->installTranslator(translator);
    }
#endif
}

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
    dirlist.append("/oem/");

    foreach (QString dirname, dirlist) {
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
    QProcess df;
    df.start("df", QStringList{"-k", "--output=source,used,avail"});
    df.waitForFinished(-1);

    QString dfout = df.readAll();

    QMap<QString, DeviceInfo> deviceInfos;
    foreach (const QString &infoline, dfout.split("\n")) {
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
        qDebug() << "device path" << devInfo.path << "used: " << devInfo.used << "total: " << devInfo.total;
        deviceInfos.insert(devInfo.path, devInfo);
    }
    return deviceInfos;
}

static QByteArray unescapeLimited(const QString &str)
{
    bool escape = false;
    QByteArray hex(2, '\0');
    QByteArray ret;
    QByteArray origin = str.toUtf8();
    for (auto it = origin.constBegin(); it != origin.constEnd();) {
        if (escape) {
            if (*it != 'x' || it + 2 > origin.constEnd())
                break;
            hex[0] = (*(it + 1));
            hex[1] = (*(it + 2));
            ret.append(QByteArray::fromHex(hex));
            escape = false;
            it += 3;
        } else {
            if (*it == '\\')
                escape = true;
            else
                ret.append(*it);
            ++it;
        }
    }

    return ret;
}

QMap<QString, DeviceInfo> CommandLsblkParse()
{
    QProcess lsblk;
    lsblk.start("lsblk", QStringList{"-b", "-p", "-P", "-o", "name,label,size,uuid,fstype,type"});
    lsblk.waitForFinished(-1);
    QString line;
    DeviceInfo info;
    QString diskDevPath;
    QMap<QString, DeviceInfo> deviceInfos;
    // currentPartPath用记录当前的磁盘设备，如sdb
    QString currentPartPath = "";
    do {
        bool isPart = false;
        line = QString::fromUtf8(lsblk.readLine());
        if (line.isEmpty())
            break;

        QString type;
        QRegExp reg("NAME=\"(.*)\" LABEL=\"(.*)\" SIZE=\"(.*)\" UUID=\"(.*)\" FSTYPE=\"(.*)\" TYPE=\"(.*)\"");

        if (reg.indexIn(line) >= 0) {
            info.path = reg.cap(1);
            info.label = reg.cap(2);
            info.uuid = reg.cap(4);
            info.fstype = reg.cap(5);
            type = reg.cap(6);

            if (!type.compare("disk")) {
                diskDevPath = info.path;
                isPart = false;
            }
            else if (!type.compare("part")){
                isPart = true;
            } else {
                diskDevPath = "";
            }
        }

        QString strLabel = info.label;

        if (info.label.contains("\\x", Qt::CaseInsensitive)) {
            QByteArray byArr = unescapeLimited(info.label);

            if (isUft8(byArr)) {
                strLabel = QTextCodec::codecForName("UTF-8")->toUnicode(byArr);
            }
            else if(isGBK(byArr)) {
                strLabel = QTextCodec::codecForName("GBK")->toUnicode(byArr);
            }
            else {
                strLabel = QString::fromLocal8Bit(byArr);
            }

            if (strLabel.isEmpty())
                strLabel = QObject::tr("Removable disk");
        }

        info.label = strLabel;
        // 如果是disk 如sdb1，sdb2。
        if (isPart && !diskDevPath.isEmpty() && (info.path.left(currentPartPath.length()) == currentPartPath)) {
            info.isDisk = false;
            info.strDev = diskDevPath;
            deviceInfos[diskDevPath].children.insert(info.path, info);
        } else { // 否则就是 part, 如sdb。
            info.isDisk = true;
            info.strDev = "";
            deviceInfos.insert(info.path, info);
            // 记录当前是part的情况
            currentPartPath = info.path;
        }
    } while(true);

    return deviceInfos;
}
#endif


bool CheckInstallDisk(const QString &targetDev)
{
    qDebug() << "CheckInstallDisk";
    if (XSys::DiskUtil::PF_FAT32 != XSys::DiskUtil::GetPartitionFormat(targetDev)) {
        qDebug() << "disk format error " << targetDev;
        return false;
    }

    QString targetPath = XSys::DiskUtil::MountPoint(targetDev);
    qDebug() << "targetPath: " << targetPath;
    QFile test(QDir::toNativeSeparators(targetPath + "/" + "UOS"));

    if (!test.open(QIODevice::ReadWrite)) {
        qDebug() << "erro open file: " << test.fileName();
        return false;
    }

    QFile UOS(":src/UOS");
    UOS.open(QIODevice::ReadOnly);
    QByteArray data = UOS.readAll();

    if (data.length() != test.write(data)) {
        qDebug() << "erro write file: " << UOS.fileName();
        return false;
    }

    test.close();
    UOS.close();
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
    qDebug() << "ListUsbDrives";
    QList<DeviceInfo> deviceList;
#ifdef Q_OS_WIN32
    QFileInfoList extdrivesList = QDir::drives();

    for (int i = 0; i < extdrivesList.size(); ++i) {
        QString deviceLetter = extdrivesList.at(i).path().toUpper();
        if (QDir::toNativeSeparators(deviceLetter) != QDir::toNativeSeparators(QDir::rootPath().toUpper()) && !QDir::toNativeSeparators(deviceLetter)
                .contains("A:") && !QDir::toNativeSeparators(deviceLetter)
                .contains("B:")) {
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

    QMap<QString, QString> removeDevice;

    for (int i = 0; i < usbfileinfoL.size(); ++i) {
        if (usbfileinfoL.at(i).fileName().contains(QRegExp("^usb-\\S{1,}$")) || usbfileinfoL.at(i).fileName().contains(QRegExp("^mmc-\\S{1,}$"))) {
            QString path = usbfileinfoL.at(i).canonicalFilePath();
            removeDevice.insert(path, usbfileinfoL.at(i).fileName());
        }
    }

    for (auto devicePath : lsblkDeviceInfos.keys()) {
        if (!removeDevice.contains(devicePath)) {
            continue;
        }
        // find first partition
        QString strDiskName = devicePath;
        DeviceInfo diskinfo = lsblkDeviceInfos.value(devicePath);
        QStringList partitionNames = diskinfo.children.keys();

        foreach (QString strPartionName, partitionNames) {
            bool needformat = true;
            DeviceInfo partitionInfo = diskinfo.children.value(strPartionName);

            if (partitionInfo.fstype != "vfat") {
                needformat = true;
            }
            else {
                needformat = false;
            }

            DeviceInfo dfinfo = dfDeviceInfos.value(strPartionName);
            if (partitionInfo.label.isEmpty()) {
                partitionInfo.label = partitionInfo.path;
            }

            partitionInfo.used = dfinfo.used;
            partitionInfo.total = dfinfo.total;
            partitionInfo.target = dfinfo.target;
            partitionInfo.needFormat = needformat;
            deviceList.push_back(partitionInfo);
            qDebug() << partitionInfo.path << partitionInfo.used << partitionInfo.total << partitionInfo.target << partitionInfo.needFormat;
        }
    }
#endif

#ifdef Q_OS_MAC
    QStringList fulldrivelist;
    QString out = XSys::FS::TmpFilePath("diskutil_out");
    XSys::SynExec("bash", QString("-c \" diskutil list > \"%1\" \" ").arg(out));
    QFile outfile(out);
    outfile.open(QIODevice::ReadOnly);
    QString diskutilList = outfile.readAll();
    QStringList usbdevsL = diskutilList.split("\n").filter(QRegExp("(FAT|Microsoft)")).join(" ").split(" ").filter("disk");

    for (int i = 0; i < usbdevsL.size(); ++i) {
        if (isUsbDisk("/dev/" + usbdevsL.at(i))) {
            auto path = "/dev/" + usbdevsL.at(i);
            fulldrivelist.append(path);

            DeviceInfo info;
            info.path = path;
            deviceList.push_back(info);
        }
    }

    qDebug() << fulldrivelist;
    outfile.close();
    outfile.remove();
#endif
    return deviceList;
}

void initResource()
{
    initQRC();
}
}
