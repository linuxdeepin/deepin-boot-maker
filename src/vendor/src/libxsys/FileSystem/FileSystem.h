// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QString>

class QFile;

namespace XSys
{
namespace FS
{
QString TmpFilePath(const QString &filename = "", const QString &distFilename = "");
QString InsertTmpFile(const QString &fileurl, const QString &distFilename = "");
bool InsertFile(const QString &fileurl, const QString &fullpath);
bool InsertFileData(const QString &name, const QByteArray &data = "");
QString SynExec(const QString &exec, const QString &param, const QString &execPipeIn = "");
bool RmFile(QFile &file);
bool RmFile(const QString &filename);
bool CpFile(const QString &srcName, const QString &desName);
bool MoveDir(const QString &oldName, const QString &newName);
bool RmDir(const QString &dirpath);

QString PathSearch(const QString &filename, const QStringList &pathlist);
QString SearchBin(const QString &binName) ;
}
}
