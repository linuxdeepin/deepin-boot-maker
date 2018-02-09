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
