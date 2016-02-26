/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#pragma once

#include <QString>

class QFile;

namespace XSys {
    namespace FS {
        QString TmpFilePath(const QString &filename = "");
        QString InsertTmpFile(const QString &fileurl);
        bool InsertFile(const QString &fileurl, const QString &fullpath);
        bool InsertFileData(const QString &name, const QByteArray &data = "");
        QString SynExec(const QString &exec, const QString &param, const QString &execPipeIn = "");
        bool RmFile(QFile &file);
        bool RmFile(const QString &filename);
        bool CpFile(const QString &srcName, const QString &desName);
        bool MoveDir(const QString &oldName, const QString &newName);
        bool RmDir(const QString &dirpath);

        QString PathSearch(const QString& filename, const QStringList& pathlist);
        QString SearchBin(const QString& binName) ;
    }
}
