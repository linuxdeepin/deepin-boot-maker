/*
 * Copyright (C) 2015 ~ 2018 Wuhan Deepin Technology Co., Ltd.
 *
 * Author:     Iceyer <me@iceyer.net>
 *
 * Maintainer: Iceyer <me@iceyer.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "utils.h"

#include <QtCore>
#include <QString>

#include <XSys>
#include <dirent.h>
#include <QTextCodec>
#include <QByteArray>
#ifdef Q_OS_WIN32
#include <Windows.h>
#endif

static void initQRC()
{
#ifdef Q_OS_LINUX
    Q_INIT_RESOURCE(blob_linux);
#else
    Q_INIT_RESOURCE(blob);
#endif
}

namespace Utils {


void loadTranslate()
{
    QTranslator *qtTranslator = new QTranslator;
    qtTranslator->load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    qApp->installTranslator(qtTranslator);

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
    do {
        bool isPart = false;
        line = QString::fromUtf8(lsblk.readLine());
        if (line.isEmpty())
            break;
        QStringList pairs = line.split(" ");
        for (auto it = pairs.constBegin(); it != pairs.constEnd(); ++it) {
            if (it->isEmpty())
                continue;

            QStringList kv = it->split("=");
            if (kv.size() != 2)
                continue;

            QString key = kv[0].toLower();
            QString value = kv[1].trimmed();
            if (value.endsWith("\"") && value.startsWith("\""))
                value = value.mid(1, value.size() - 2);

            if (!key.compare("name")) {
                info.path = value;
            } else if (!key.compare("label")) {
                if (value.contains("\\x", Qt::CaseInsensitive)) {
                    value = QTextCodec::codecForName("GBK")->toUnicode(unescapeLimited(value));
                    if (value.isEmpty())
                        value = QObject::tr("Removable disk");
                }
                info.label = value;
            }
            else if (!key.compare("uuid")) {
                info.uuid = value;
            } else if (!key.compare("fstype")) {
                info.fstype = value;
            } else if (!key.compare("type")) {
                QString type = value;
                if (!type.compare("disk"))
                    diskDevPath = info.path;
                else if (!type.compare("part")){
                    isPart = true;
                } else {
                    diskDevPath = "";
                }
            }
        }
        if (isPart && !diskDevPath.isEmpty()) {
            deviceInfos[diskDevPath].children.insert(info.path, info);
        } else {
            deviceInfos.insert(info.path, info);
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
        // find first partion
        QString partionPath = devicePath;
        DeviceInfo info = lsblkDeviceInfos.value(devicePath);
        bool needformat = true;

        if (!info.children.keys().isEmpty()) {
            auto infoKey = info.children.keys().value(0);
            info = info.children.value(infoKey);
            partionPath = infoKey;
            if (info.fstype != "vsfat") {
                needformat = false;
            }
        }

        DeviceInfo dfinfo = dfDeviceInfos.value(partionPath, info);
        if (info.label.isEmpty()) {
            info.label = info.path;
        }
        info.used = dfinfo.used;
        info.total = dfinfo.total;
        info.target = dfinfo.target;
        info.needFormat = needformat;

        deviceList.push_back(info);
        qDebug() << info.path << info.used << info.total << info.target << info.needFormat;
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
