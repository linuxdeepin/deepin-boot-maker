// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "Syslinux.h"

#include "../FileSystem/FileSystem.h"
#include "../Cmd/Cmd.h"

#include <QDebug>
#include <QDir>

namespace XSys {

namespace Syslinux {

#ifdef Q_OS_LINUX
QString SearchMbr()
{
    QStringList paths;
    paths.push_back("/usr/lib/syslinux/mbr/");
    paths.push_back("/usr/lib/SYSLINUX/"); // debian
    paths.push_back("/usr/lib/syslinux/"); // ubuntu
    paths.push_back("/usr/lib/syslinux/bios/"); // arch
    paths.push_back("/usr/share/syslinux/"); //euler
    return XSys::FS::PathSearch("mbr.bin", paths);
}
#endif

QString SearchMoudle(const QString &moduleName)
{
#ifdef Q_OS_LINUX
    QStringList paths;
    paths.push_back("/usr/lib/syslinux/modules/bios/"); //debain
    paths.push_back("/usr/lib/syslinux/"); //ubuntu
    paths.push_back("/usr/lib/syslinux/bios/"); //arch
    paths.push_back("/usr/share/syslinux/"); //euler
    return XSys::FS::PathSearch(moduleName, paths);
#endif

#ifdef Q_OS_MAC
    return ":blob/syslinux/macosx/" + moduleName;
#endif

#ifdef Q_OS_WIN32
    return ":blob/syslinux/win32/" + moduleName;
#endif
}


Result InstallBootloader(const QString &device)
{
#ifdef Q_OS_LINUX
    return XSys::SynExec(XSys::FS::SearchBin("syslinux"), QString(" -i %1").arg(device));
#else
    Q_UNUSED(device);
    return Result(Result::Success, "", "");
#endif
}

Result InstallMbr(const QString &device)
{
#ifdef Q_OS_LINUX
    QString mbrPath = SearchMbr();
    return XSys::SynExec("dd", QString(" if=%1 of=%2 bs=440 count=1").arg(mbrPath).arg(device));
#else
    Q_UNUSED(device);
    return Result(Result::Success, "", "");
#endif
}

Result InstallModule(const QString &installDirectory)
{
    QStringList modulelist;
    modulelist.append("gfxboot.c32");
    modulelist.append("chain.c32");
    modulelist.append("menu.c32");
    modulelist.append("vesamenu.c32");
#ifndef Q_OS_MAC
    modulelist.append("libcom32.c32");
    modulelist.append("libutil.c32");
#endif

    foreach (QString filename, modulelist) {
        qDebug() << "Install Module" << filename << SearchMoudle(filename);
        if (!XSys::FS::InsertFile(SearchMoudle(filename), QDir::toNativeSeparators(installDirectory + filename))) {
            return Result(Result::Failed, "Insert Module File Failed: " + SearchMoudle(filename) + " to " + QDir::toNativeSeparators(installDirectory + filename));
        }
    }

    return Result(Result::Success, "", "");
}

Result ConfigSyslinx(const QString &targetPath)
{
    // rename isolinux to syslinux
    QString syslinxDir = QString("%1/syslinux/").arg(targetPath);
    if (!XSys::FS::RmDir(syslinxDir)) {
        return Result(Result::Failed, "Remove Dir Failed: " + syslinxDir);
    }

    QString isolinxDir = QString("%1/isolinux/").arg(targetPath);
    if (!XSys::FS::MoveDir(isolinxDir, syslinxDir)) {
        return Result(Result::Failed, "Move Dir Failed: " + isolinxDir + " to " + syslinxDir);
    }
    qDebug() << "Move " << isolinxDir << " ot " << syslinxDir;

    QString syslinxCfgPath = QString("%1/syslinux/syslinux.cfg").arg(targetPath);
    if (!XSys::FS::RmFile(syslinxCfgPath)) {
        return Result(Result::Failed, "Remove File Failed: " + syslinxCfgPath);
    }

    QString isolinxCfgPath = QString("%1/syslinux/isolinux.cfg").arg(targetPath);
    qDebug() << "Rename " << isolinxCfgPath << " ot " << syslinxCfgPath;

    if (!XSys::FS::CpFile(isolinxCfgPath, syslinxCfgPath)) {
        return Result(Result::Failed, "Copy File Failed: " + isolinxCfgPath + " to " + syslinxCfgPath);
    }

    qDebug() << "InstallModule to" << syslinxDir;
    XSys::Syslinux::InstallModule(syslinxDir);

    // bugfix
    // TODO: we change syslinux to 6.02, but gfxboot will not work
    // so use a syslinux.cfg will not use gfxboot and vesamenu
//    if (!XSys::FS::InsertFile(":/blob/syslinux/syslinux.cfg", QDir::toNativeSeparators(syslinxDir + "syslinux.cfg"))) {
//        return Result(Result::Failed, "Insert Config File Failed: :/blob/syslinux/syslinux.cfg to " + QDir::toNativeSeparators(syslinxDir + "syslinux.cfg"));
//    }

    return Result(Result::Success, "");
}


}

}
