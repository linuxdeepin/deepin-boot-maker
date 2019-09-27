/*
 * Copyright (C) 2017 ~ 2018 Wuhan Deepin Technology Co., Ltd.
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

#ifndef SEVENZIP_H
#define SEVENZIP_H

#include <QObject>
#include <QThread>
#include <QProcess>

class SevenZipProcessParser : public QThread
{
    Q_OBJECT
public:
    explicit SevenZipProcessParser(const QString &file, QProcess *process, QObject *parent = 0);

    void setProgressName(const QString &file) {m_progressFilename = file;}

    void run() Q_DECL_OVERRIDE;

signals:
    void progressChanged(int, int, const QString &);

private:
    QString     m_progressFilename;
    QProcess    *m_sevenZip = nullptr;

    int         m_lastPencent = 0;
    QString     m_lastFilename;
};

class SevenZip : public QObject
{
    Q_OBJECT
public:
    SevenZipProcessParser   *m_szpp;

    explicit SevenZip(const QString &image, const QString &target, QObject *parent = 0);

    void setArchiveFile(const QString &archiveFile);
    void setOutputDirectory(const QString &outputDir);

    bool extract();
    bool check();

signals:
    void progressChanged(int);

private:
    QProcess                m_sevenz;
    QString                 m_sevenZip;
    QString                 m_archiveFile;
    QString                 m_outputDir;
};

#endif // SEVENZIP_HHH
