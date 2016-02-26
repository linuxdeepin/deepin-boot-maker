/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#pragma once

#include <QObject>
#include <QString>

#include "../Common/Result.h"

namespace XSys {

namespace DiskUtil {
    enum PartionFormat {
        PF_FAT32,
        PF_NTFS,
        PF_RAW,
    };

    bool UmountDisk(const QString &targetDev);
    bool EjectDisk(const QString &targetDev);

    bool Mount(const QString& targetDev, const QString& path);
    QString MountPoint(const QString& targetDev) ;
    PartionFormat GetPartitionFormat(const QString &targetDev);
    QString GetPartitionDisk(const QString &targetDev);

    qint64 GetPartitionFreeSpace(const QString &targetDev);
    QString GetPartitionLabel(const QString &targetDev);
}

namespace Bootloader {

    Result InstallBootloader(const QString &diskDev);

    namespace Syslinux {
        Result InstallSyslinux(const QString &diskDev);
        Result ConfigSyslinx(const QString &targetDev);
    }

    }
}
