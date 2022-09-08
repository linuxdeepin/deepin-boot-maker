// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once
#include "../Common/Result.h"

#include <QObject>
#include <QString>

namespace XSys {

namespace DiskUtil {
enum PartionFormat {
    PF_FAT32,
    PF_NTFS,
    PF_RAW,
};

bool UmountDisk(const QString &targetDev);
bool UmountPartion(const QString& strPartionName);
XSys::Result EjectDisk(const QString &targetDev);
void detach();
bool Mount(const QString &targetDev);
bool Mount(const QString &targetDev, const QString &path);
QString MountPoint(const QString &targetDev) ;
PartionFormat GetPartitionFormat(const QString &targetDev);
QString GetPartitionDisk(const QString &targetDev);

qint64 GetPartitionFreeSpace(const QString &targetDev);
qint64 GetPartitionTotalSpace(const QString &targetDev);
QString GetPartitionLabel(const QString &targetDev);
bool FormatPartion(const QString& targetDev);
QStringList GetPartionOfDisk(const QString& strDisk);
bool SetActivePartion(const QString& strDisk, const QString& strPartion);
void SetPartionLabel(const QString& strPartion, const QString& strImage);
QString getPartitionUUID(const QString& strPartition);
}

namespace Bootloader {

Result InstallBootloader(const QString &diskDev, const QString &images);
}
}
