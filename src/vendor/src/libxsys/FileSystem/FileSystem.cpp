// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "FileSystem.h"

#include <QDebug>
#include <QStandardPaths>
#include <QTime>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QCryptographicHash>
#include <QRandomGenerator>

static QString randString(const QString &str)
{
    QString seedStr = str + 
    QTime::currentTime().toString(Qt::TextDate) + QString("%1").arg(QRandomGenerator::global()->generate());
    return QString("").append(QCryptographicHash::hash(seedStr.toLatin1(), QCryptographicHash::Md5).toHex());
}

namespace XSys {
namespace FS {

QString TmpFilePath(const QString &filename, const QString &distFilename)
{
//fix bug 16574 非开发者模式第三方程序无法签名的问题，现将第三放程序写到/usr/bin下面，mac平台保持原样
#ifdef Q_OS_LINUX
    QString tmpDir = "/usr/bin/";
#endif
#ifndef Q_OS_LINUX
    QString tmpDir = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).first();
#endif
    static bool init = QDir(tmpDir).mkdir("xsys");
    Q_UNUSED(init);
    QFileInfo fi(filename);
    QString ext = "";
    if (!fi.suffix().isEmpty())  {
        ext = "." + fi.suffix();
    }
    QString newFilename  = distFilename;
    if (newFilename.isEmpty()) {
        newFilename = randString(filename) + ext;
    }
    qDebug() << "request create usr file" << filename << distFilename;
    auto tmppath = QDir::toNativeSeparators(QString(tmpDir + "/xsys/" + newFilename));
    qDebug() << "create usr file" << tmppath;
    return tmppath;
}

bool InsertFileData(const QString &filename, const QByteArray &data)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Insert Tmp FileData Failed, Can not open" << filename;
        return false;
    }
    if (!file.write(data)) {
        qWarning() << "Insert Tmp FileData Failed, Can not Write" << filename;
        file.close();
        return false;
    }
    file.close();
    return true;
#ifdef Q_OS_UNIX
    // SynExec("sync", "");
#endif
}

QString InsertTmpFile(const QString &fileurl, const QString &distFilename)
{
    QString filename = TmpFilePath(fileurl, distFilename);
    QFile file(fileurl);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Insert Tmp File Failed, Can not open" << fileurl;
        return filename;
    }
    if (!InsertFileData(filename, file.readAll())) {
        qWarning() << "Insert Tmp File Failed" << fileurl;
        file.close();
        return filename;
    }
    file.close();
    return filename;
}

bool InsertFile(const QString &fileurl, const QString &fullpath)
{
    QFile file(fileurl);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    if (!InsertFileData(fullpath, file.readAll())) {
        file.close();
        return false;
    }
    file.close();
    return true;
}

bool RmFile(QFile &fn)
{
    if (!fn.exists()) {
        return true;
    }
    fn.setPermissions(QFile::WriteUser);
    return fn.remove();
#ifdef Q_OS_UNIX
    //SynExec("sync", "");
#endif
}

bool RmFile(const QString &filename)
{
    QFile file(filename);
    return RmFile(file);
}

bool CpFile(const QString &srcName, const QString &desName)
{
    bool ret = true;
    QFile srcFile(srcName);
    QFile desFile(desName);
    if (!srcFile.open(QIODevice::ReadOnly)) {
        qWarning() << "error open file: " << srcName;
        return false;
    }
    if (!desFile.open(QIODevice::WriteOnly)) {
        qWarning() << "error open file: " << desName;
        srcFile.close();
        return false;
    }
    QByteArray data = srcFile.readAll();
    qint64 writeBytes = desFile.write(data);
    if (writeBytes != data.size()) {
        qWarning() << "Copy File Failed, " << srcName << " to " << desName;
        ret = false;
    }
    srcFile.close();
    desFile.close();
#ifdef Q_OS_UNIX
    //SynExec("sync", "");
#endif
    return ret;
}

bool RmDir(const QString &dirpath)
{
    bool result = true;
    QDir dir(dirpath);

    if (dir.exists(dirpath)) {
        Q_FOREACH (QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = RmDir(info.absoluteFilePath());
            } else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirpath);
    }
#ifdef Q_OS_UNIX
    // SynExec("sync", "");
#endif
    return result;
}

bool MoveDir(const QString &oldName, const QString &newName)
{
    RmFile(newName);
    RmDir(newName);
    QDir dir(oldName);
    return dir.rename(oldName, newName);

#ifdef Q_OS_UNIX
    //SynExec("sync", "");
#endif
}

QString PathSearch(const QString &filename, const QStringList &pathlist)
{
    for (int i = 0; i < pathlist.length(); ++i) {
        QDir path(pathlist.at(i));
        QFile file(path.absoluteFilePath(filename));
        if (file.exists()) {
            return file.fileName();
        }
    }
    return "";
}

QString SearchBin(const QString &binName)
{
    QStringList paths;
    paths.push_back("/sbin/");
    paths.push_back("/usr/sbin/");
    paths.push_back("/bin/");
    paths.push_back("/usr/bin/");
    paths.push_back("/usr/local/bin/");
    return PathSearch(binName, paths);
}

}
}
