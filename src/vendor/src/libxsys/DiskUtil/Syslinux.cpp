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

#include "Syslinux.h"

#include <QDebug>
#include <QDir>

#include "../FileSystem/FileSystem.h"
#include "../Cmd/Cmd.h"

namespace XSys {

namespace Syslinux {

#ifdef Q_OS_LINUX
QString SearchMbr() {
    QStringList paths;
    paths.push_back("/usr/lib/syslinux/mbr/");
    paths.push_back("/usr/lib/SYSLINUX/"); // debian
    paths.push_back("/usr/lib/syslinux/"); // ubuntu
    paths.push_back("/usr/lib/syslinux/bios/"); // arch
    return XSys::FS::PathSearch("mbr.bin", paths);
}
#endif

QString SearchMoudle(const QString& moduleName) {
#ifdef Q_OS_LINUX
    QStringList paths;
    paths.push_back("/usr/lib/syslinux/modules/bios/"); //debain
    paths.push_back("/usr/lib/syslinux/"); //ubuntu
    paths.push_back("/usr/lib/syslinux/bios/"); //arch
    return XSys::FS::PathSearch(moduleName, paths);
#endif

#ifdef Q_OS_MAC
    return ":blob/syslinux/macosx/" + moduleName;
#endif

#ifdef Q_OS_WIN32
    return ":blob/syslinux/win32/" + moduleName;
#endif
}


Result InstallBootloader(const QString &device) {
#ifdef Q_OS_LINUX
    return XSys::SynExec(XSys::FS::SearchBin("syslinux"), QString(" -i %1").arg(device));
#else
    Q_UNUSED(device);
    return Result(Result::Success, "", "");
#endif
}

Result InstallMbr(const QString &device) {
#ifdef Q_OS_LINUX
    QString mbrPath = SearchMbr();
    return XSys::SynExec("dd", QString(" if=%1 of=%2 bs=440 count=1").arg(mbrPath).arg(device));
#else
    Q_UNUSED(device);
    return Result(Result::Success, "", "");
#endif
}

Result InstallModule(const QString& installDirectory) {
    QStringList modulelist;
    modulelist.append("gfxboot.c32");
    modulelist.append("chain.c32");
    modulelist.append("menu.c32");
    modulelist.append("vesamenu.c32");
#ifndef Q_OS_MAC
    modulelist.append("libcom32.c32");
    modulelist.append("libutil.c32");
#endif

    foreach(QString filename, modulelist) {
        qDebug()<<"Install Module"<<filename<<SearchMoudle(filename);
        if (!XSys::FS::InsertFile(SearchMoudle(filename), QDir::toNativeSeparators(installDirectory + filename))) {
            return Result(Result::Faiiled, "Insert Module File Failed: " + SearchMoudle(filename) + " to " + QDir::toNativeSeparators(installDirectory + filename));
        }
    }

    return Result(Result::Success, "", "");
}


}

}
