// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "utils.h"

#include <QtCore>
#include <QString>
#include <QProcess>

#include <XSys>
#include <dirent.h>
#include <QByteArray>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QRegExp>
#include <QTextCodec>
#else
#include <QRegularExpression>
#endif

#include <qglobal.h>
#ifdef Q_OS_WIN32
#    include <Windows.h>
#endif

static void initQRC()
{
    qDebug() << "Initializing resource files";
#ifdef Q_OS_LINUX
    //Q_INIT_RESOURCE(blob_linux);
#else
    Q_INIT_RESOURCE(blob);
#endif
}

namespace Utils {

bool isUft8(const QByteArray &byArr)
{
    qDebug() << "Checking UTF-8 encoding";
    unsigned int nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
    bool bAllAscii = true;

    for (int i = 0; i < byArr.length(); ++i) {
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
                } else if (chr >= 0xF8) {
                    nBytes = 5;
                } else if (chr >= 0xF0) {
                    nBytes = 4;
                } else if (chr >= 0xE0) {
                    nBytes = 3;
                } else if (chr >= 0xC0) {
                    nBytes = 2;
                }
                else {
                    qDebug() << "Invalid UTF-8 sequence detected";
                    return false;
                }

                nBytes--;
            }
        } else {
            if ((chr & 0xC0) != 0x80) {
                qDebug() << "Invalid UTF-8 continuation byte";
                return false;
            }

            nBytes--;
        }
    }

    if (nBytes != 0)  {
        qDebug() << "Incomplete UTF-8 sequence";
        return false;
    }

    if (bAllAscii) {   //如果全部都是ASCII, 也是UTF8
        return true;
    }

    return true;
}

bool isGBK(const QByteArray &byArr)
{
    qDebug() << "Checking GBK encoding";
    unsigned int nBytes = 0;
    bool bAllAscii = true;

    for (int i = 0; i < byArr.length(); ++i) {
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
                    qDebug() << "Invalid GBK sequence";
                    return false;
                }

                nBytes--;
            }
        }
        else {
            if (chr < 0x40 || chr>0xFE) {
                qDebug() << "Invalid GBK continuation byte";
                return false;
            }

            nBytes--;
        }
    }

    if (nBytes != 0) {//违返规则
        qDebug() << "Incomplete GBK sequence";
        return false;
    }

    if (bAllAscii) {   //如果全部都是ASCII, 也是GBK
        return true;
    }

    return true;
}

void loadTranslate()
{
    qDebug() << "Loading translations";
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
    QStringList allappargs = qApp->arguments();
    QList<QPair<QString, QString>> oppairs;

    for (QList<QString>::const_iterator i = allappargs.constBegin(); i < allappargs.constEnd(); ++i) {
        if (i->count('=') == 1) {
            oppairs.append(QPair<QString, QString>(i->section('=', 0, 0).simplified(), i->section('=', 1, 1).simplified()));
        }
    }

    for (QList<QPair<QString, QString>>::const_iterator i = oppairs.constBegin(); i < oppairs.constEnd(); ++i) {
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

    qDebug() << "System locale:" << clangcode;
    qDebug() << "Loading translation file:" << tranlateUrl;

    if (translator->load(tranlateUrl)) {
        qApp->installTranslator(translator);
        qDebug() << "Translation loaded successfully";
    } else {
        qWarning() << "Failed to load translation file:" << tranlateUrl;
    }
#endif
}

QString UsbShowText(const QString &dev)
{
    qDebug() << "Getting USB device display text for:" << dev;
    QString showText;
    if (!dev.isEmpty()) {
        QString label = XSys::DiskUtil::GetPartitionLabel(dev);
        if (label.isEmpty()) {
            qDebug() << "No label found, using default label";
            label = QObject::tr("Removable disk");
        }
#ifdef Q_OS_UNIX
        QFileInfo devFile(dev);
        showText = QString("%1: %2").arg(devFile.baseName()).arg(label);
#endif
#ifdef Q_OS_WIN32
        showText = QString("(%1:) %2").arg(dev.at(0)).arg(label);
#endif
        qDebug() << "Device display text:" << showText;
    }
    return showText;
}

void ClearTargetDev(const QString &targetPath)
{
    qInfo() << "Clearing target device at:" << targetPath;
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
        qDebug() << "Removing directory:" << fullDir;
        XSys::FS::RmDir(fullDir);
    }

#ifdef Q_OS_UNIX
    qDebug() << "Syncing filesystem";
    XSys::SynExec("sync", "");
#endif
}

#ifdef Q_OS_UNIX
QMap<QString, DeviceInfo> CommandDfParse()
{
    qDebug() << "Step 2: Execute df command to get disk usage";
    QProcess df;
    // 排除网络文件系统类型，避免因为远程挂载卡死
    df.start("df", QStringList{"-k", "--output=source,used,avail",
                               "-x", "nfs",
                               "-x", "cifs",
                               "-x", "smbfs",
                               "-x", "sshfs",
                               "-x", "ftpfs"});
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
        devInfo.total = static_cast<quint32>((infos.at(2).toInt() + infos.at(1).toInt()) / 1024);
        qDebug() << "device path" << devInfo.path << "used: " << devInfo.used << "total: " << devInfo.total;
        deviceInfos.insert(devInfo.path, devInfo);
    }
    qDebug() << "Step 2 completed: df command finished, got" << deviceInfos.size() << "disk info";
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
    qDebug() << "Step 3: Execute lsblk command to get device details";
    QProcess lsblk;
    lsblk.start("lsblk", QStringList { "-b", "-p", "-P", "-o", "name,label,size,uuid,fstype,type" });
    lsblk.waitForFinished(-1);
    qDebug() << "Step 3 completed: lsblk command finished";
    QString line;
    DeviceInfo info;
    QString diskDevPath;
    QMap<QString, DeviceInfo> deviceInfos;
    // currentPartPath用记录当前的磁盘设备，如sdb
    QString currentPartPath = "";
    int deviceCount = 0;
    do {
        bool isPart = false;
        line = QString::fromUtf8(lsblk.readLine());
        if (line.isEmpty())
            break;

        QString type;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QRegExp reg("NAME=\"(.*)\" LABEL=\"(.*)\" SIZE=\"(\\d+)\" UUID=\"(.*)\" FSTYPE=\"(.*)\" TYPE=\"(.*)\"");
        if (reg.indexIn(line) >= 0) {
            deviceCount++;
            info.path = reg.cap(1);
            info.label = reg.cap(2);
            info.total = reg.cap(3).toLongLong() * 1024 / 1024; // MB
            info.uuid = reg.cap(4);
            info.fstype = reg.cap(5);
            type = reg.cap(6);
#else
        QRegularExpression reg("NAME=\"(.*)\" LABEL=\"(.*)\" SIZE=\"(.*)\" UUID=\"(.*)\" FSTYPE=\"(.*)\" TYPE=\"(.*)\"");
        QRegularExpressionMatch match = reg.match(line);
        if (match.hasMatch()) {
            info.path = match.captured(1);
            info.label = match.captured(2);
            info.uuid = match.captured(4);
            info.fstype = match.captured(5);
            type = match.captured(6);
#endif
            qDebug() << "Parse device" << deviceCount << ":" << info.path << "Type:" << type;

            if (!type.compare("disk")) {
                diskDevPath = info.path;
                isPart = false;
            } else if (!type.compare("part")) {
                isPart = true;
            } else {
                diskDevPath = "";
            }
        }

        QString strLabel = info.label;

        if (info.label.contains("\\x", Qt::CaseInsensitive)) {
            QByteArray byArr = unescapeLimited(info.label);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            if (isUft8(byArr)) {
                strLabel = QTextCodec::codecForName("UTF-8")->toUnicode(byArr);
            } else if (isGBK(byArr)) {
                strLabel = QTextCodec::codecForName("GBK")->toUnicode(byArr);
            }
#else
            if (isUft8(byArr)) {
                strLabel = QString::fromUtf8(byArr);
            }
            else if(isGBK(byArr)) {
                strLabel = QString::fromLocal8Bit(byArr);
            }
#endif
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
            qDebug() << "  Add partition to device" << diskDevPath << ":" << info.path;
            deviceInfos[diskDevPath].children.insert(info.path, info);
        } else {   // 否则就是 part, 如sdb。
            info.isDisk = true;
            info.strDev = "";
            qDebug() << "  Add disk device:" << info.path;
            deviceInfos.insert(info.path, info);
            // 记录当前是part的情况
            currentPartPath = info.path;
        }
    } while(true);
    qDebug() << "Step 2 completed: Parse lsblk output finished, total" << deviceCount << "devices parsed";

    return deviceInfos;
}
#endif

bool CheckInstallDisk(const QString &targetDev)
{
    qDebug() << "Checking installation disk:" << targetDev;
    if (XSys::DiskUtil::PF_FAT32 != XSys::DiskUtil::GetPartitionFormat(targetDev)) {
        qWarning() << "Invalid disk format for:" << targetDev;
        return false;
    }

    QString targetPath = XSys::DiskUtil::MountPoint(targetDev);
    qDebug() << "Target mount point:" << targetPath;
    QFile test(QDir::toNativeSeparators(targetPath + "/" + "UOS"));

    if (!test.open(QIODevice::ReadWrite)) {
        qWarning() << "Failed to open test file:" << test.fileName();
        return false;
    }

    QFile UOS(":src/UOS");
    UOS.open(QIODevice::ReadOnly);
    QByteArray data = UOS.readAll();

    if (data.length() != test.write(data)) {
        qWarning() << "Failed to write test file:" << UOS.fileName();
        return false;
    }

    test.close();
    UOS.close();
    test.remove();
    qInfo() << "Installation disk check passed";
    return true;
}

bool isUsbDisk(const QString &dev)
{
    qDebug() << "Checking if device is USB:" << dev;
    QString out = XSys::FS::TmpFilePath("diskutil_isusb_out");
    XSys::SynExec("diskutil", QString(" \"  info %1 > \"%2\" \" ").arg(dev).arg(out));
    QFile outfile(out);
    outfile.open(QIODevice::ReadOnly);
    QString info = outfile.readAll();
    outfile.close();
    outfile.remove();
    bool isUsb = info.contains(QRegularExpression("Protocol:\\s+USB"));
    qDebug() << "Device" << dev << "is USB:" << isUsb;
    return isUsb;
}

QList<DeviceInfo> ListUsbDrives()
{
    qDebug() << "Listing USB drives";
    qDebug() << "Start getting device list";
    QList<DeviceInfo> deviceList;
#ifdef Q_OS_WIN32
    QFileInfoList extdrivesList = QDir::drives();

    for (int i = 0; i < extdrivesList.size(); ++i) {
        QString deviceLetter = extdrivesList.at(i).path().toUpper();
        if (QDir::toNativeSeparators(deviceLetter) != QDir::toNativeSeparators(QDir::rootPath().toUpper()) && !QDir::toNativeSeparators(deviceLetter).contains("A:") && !QDir::toNativeSeparators(deviceLetter).contains("B:")) {
            if (GetDriveType(LPWSTR(deviceLetter.utf16())) == 2) {
                qDebug() << "Found removable drive:" << deviceLetter;
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
    qDebug() << "Step 1: Scan /dev/disk/by-id/ directory, found" << usbfileinfoL.size() << "device files";

    QMap<QString, DeviceInfo> dfDeviceInfos = CommandDfParse();
    QMap<QString, DeviceInfo> lsblkDeviceInfos = CommandLsblkParse();

    QMap<QString, QString> removeDevice;

    for (int i = 0; i < usbfileinfoL.size(); ++i) {
        QString fileName = usbfileinfoL.at(i).fileName();
        QString filePath = usbfileinfoL.at(i).canonicalFilePath();

        // 过滤USB和MMC设备，但排除网络相关设备
        bool isUsbOrMmc = fileName.contains(QRegExp("^usb-\\S{1,}$")) || fileName.contains(QRegExp("^mmc-\\S{1,}$"));
        bool isNetwork = fileName.contains(QRegExp("nfs-|cifs-|smbfs-|sshfs-|ftpfs-|davfs-"));

        if (isUsbOrMmc && !isNetwork) {
            removeDevice.insert(filePath, fileName);
        }
    }
    qDebug() << "Step 4: Filter devices - Found" << removeDevice.size() << "USB/MMC devices from" << usbfileinfoL.size() << "total devices";

    for (auto it = removeDevice.begin(); it != removeDevice.end(); ++it) {
        qDebug() << "  Device path:" << it.key() << "Device name:" << it.value();
    }

    qDebug() << "Step 5: Traverse device partitions and filter eligible devices";
    int processedCount = 0;
    int validCount = 0;

    for (auto devicePath : lsblkDeviceInfos.keys()) {
        processedCount++;
        if (!removeDevice.contains(devicePath)) {
            continue;
        }
        qDebug() << "Processing device" << processedCount << ":" << devicePath << "with" << lsblkDeviceInfos.value(devicePath).children.size() << "partitions";
        // find first partition
        QString strDiskName = devicePath;
        DeviceInfo diskinfo = lsblkDeviceInfos.value(devicePath);
        QStringList partitionNames = diskinfo.children.keys();

        if (partitionNames.isEmpty()) {
            // 设备没有分区，作为整个磁盘展示
            qDebug() << "  Device" << devicePath << "has no partitions, showing as whole disk";
            DeviceInfo wholeDevice = diskinfo;
            wholeDevice.isDisk = true;
            wholeDevice.strDev = "";
            wholeDevice.label = wholeDevice.label.isEmpty() ? QObject::tr("Removable disk") : wholeDevice.label;
            wholeDevice.needFormat = (wholeDevice.fstype != "vfat");

            DeviceInfo dfinfo = dfDeviceInfos.value(devicePath);
            if (wholeDevice.total == 0)
                wholeDevice.total = dfinfo.total;
            wholeDevice.used = dfinfo.used;

            deviceList.push_back(wholeDevice);
            validCount++;
            qDebug() << "    Added to device list (item" << validCount << "):" << wholeDevice.path
                       << "Label:" << wholeDevice.label
                       << "Size:" << wholeDevice.total << "MB"
                       << "Need format:" << wholeDevice.needFormat;
        } else {
            // 设备有分区，遍历分区
            foreach (QString strPartionName, partitionNames) {
                bool needformat = true;
                DeviceInfo partitionInfo = diskinfo.children.value(strPartionName);

                qDebug() << "  Check partition" << strPartionName << "filesystem:" << partitionInfo.fstype;

                if (partitionInfo.fstype != "vfat") {
                    needformat = true;
                    qDebug() << "    Filesystem is not vfat, need format";
                }
                else {
                    needformat = false;
                    qDebug() << "    Filesystem is vfat, no need to format";
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
                validCount++;
                qDebug() << "    Added to device list (item" << validCount << "):" << partitionInfo.path
                           << "Size:" << partitionInfo.total << "MB"
                           << "Need format:" << needformat;
                qDebug() << partitionInfo.path << partitionInfo.used << partitionInfo.total << partitionInfo.target << partitionInfo.needFormat;
            }
        }
    }
    qDebug() << "Step 5 completed: Processed" << processedCount << "devices, added" << validCount << "partitions to list";
#endif

#ifdef Q_OS_MAC
    QStringList fulldrivelist;
    QString out = XSys::FS::TmpFilePath("diskutil_out");
    XSys::SynExec("diskutil", QString(" \"  list > \"%1\" \" ").arg(out));
    QFile outfile(out);
    outfile.open(QIODevice::ReadOnly);
    QString diskutilList = outfile.readAll();

    QStringList usbdevsL = diskutilList.split("\n").filter(QRegularExpression("(FAT|Microsoft)")).join(" ").split(" ").filter("disk");

    for (int i = 0; i < usbdevsL.size(); ++i) {
        if (isUsbDisk("/dev/" + usbdevsL.at(i))) {
            auto path = "/dev/" + usbdevsL.at(i);
            qDebug() << "Found USB device:" << path;
            fulldrivelist.append(path);

            DeviceInfo info;
            info.path = path;
            deviceList.push_back(info);
        }
    }

    qDebug() << "Found USB devices:" << fulldrivelist;
    outfile.close();
    outfile.remove();
#endif
    qDebug() << "Get device list completed: Found" << deviceList.size() << "available devices";
    for (int i = 0; i < deviceList.size(); ++i) {
        qDebug() << "  Device" << (i+1) << ":" << deviceList.at(i).path
                   << "Label:" << deviceList.at(i).label
                   << "Filesystem:" << deviceList.at(i).fstype
                   << "Size:" << deviceList.at(i).total << "MB"
                   << "Used:" << deviceList.at(i).used << "MB"
                   << "Need format:" << deviceList.at(i).needFormat;
    }
    return deviceList;
}

void initResource()
{
    qDebug() << "Initializing application resources";
    initQRC();
}
}
