// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#ifndef SEVENZIP_H
#define SEVENZIP_H

#include <QObject>
#include <QThread>
#include <QProcess>
#include <QEventLoop>

class SevenZipProcessParser : public QThread
{
    Q_OBJECT
public:
    explicit SevenZipProcessParser(const QString &file, QProcess *process, QObject *parent = nullptr);

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

    explicit SevenZip(const QString &image, const QString &target, QObject *parent = nullptr);

    void setArchiveFile(const QString &archiveFile);
    void setOutputDirectory(const QString &outputDir);
    void stopProcess();
    bool extract();
    bool check();

signals:
    void progressChanged(int);

private slots:
    void handleFinished();

private:
    QProcess                m_sevenz;
    QString                 m_sevenZip;
    QString                 m_archiveFile;
    QString                 m_outputDir;
    QEventLoop              m_eventLoop;
    bool m_bExit;
};

#endif // SEVENZIP_HHH
