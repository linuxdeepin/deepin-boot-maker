// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "sevenzip.h"

#include <QDebug>
#include <QDir>
#include <QThread>
#include <QProcess>
#include <QTemporaryFile>
#include <QCoreApplication>

#include <XSys>

SevenZip::SevenZip(const QString &image, const QString &target, QObject *parent)
    : QObject(parent)
    ,m_eventLoop(this)
    ,m_bExit(true)
    ,m_sevenz(this)
{
    qDebug() << "Initializing SevenZip with image:" << image << "target:" << target;

#ifdef Q_OS_LINUX
    QString sevnz = "7z";
#endif

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LANG", "zh_CN.UTF-8"); // Add an environment variable
    m_sevenz.setProcessEnvironment(env);
    m_szpp = new SevenZipProcessParser("", &m_sevenz, this);
    m_sevenZip = sevnz;
    m_archiveFile = image;
    m_outputDir = "-o" + target;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    connect(&m_sevenz, static_cast<void(QProcess::*)(int)>(&QProcess::finished),
            this, &SevenZip::handleFinished);
#else
    connect(&m_sevenz, &QProcess::finished,
            this, &SevenZip::handleFinished);
#endif
}

void SevenZip::setArchiveFile(const QString &archiveFile)
{
    qDebug() << "Setting archive file:" << archiveFile;
    m_archiveFile = archiveFile;
}

void SevenZip::setOutputDirectory(const QString &outputDir)
{
    qDebug() << "Setting output directory:" << outputDir;
    m_outputDir = "-o" + outputDir;
}

void SevenZip::stopProcess()
{
    QString strCmd = QString("kill 9 %1").arg(m_sevenz.processId());
    qDebug() << "Stopping 7-Zip process with command:" << strCmd;
    QProcess::execute(strCmd);
}

bool SevenZip::extract()
{
    qDebug() << "Starting extraction of archive:" << m_archiveFile;
    m_sevenz.setProgram(m_sevenZip);

    QTemporaryFile progress;
    progress.open();

    QStringList args;
    args << "x" << "-y"
         << m_archiveFile
         << m_outputDir
#ifndef Q_OS_MAC
         << "-bsp2";
#else
         ;
#endif

    QStringList env = QProcess::systemEnvironment();
    env << "VDPAU_DRIVER=va_gl";

    m_sevenz.setArguments(args);
    m_sevenz.setStandardErrorFile(progress.fileName());

    qDebug() << "Executing 7-Zip command:" << m_sevenz.program() << m_sevenz.arguments().join(" ");
    m_sevenz.start();
    m_sevenz.waitForStarted(-1);

#ifdef Q_OS_LINUX
    QProcess::execute(QString("ionice -c3 -p %1").arg(m_sevenz.processId()));
#endif

    m_szpp->setProgressName(progress.fileName());
    m_szpp->start();
    m_eventLoop.exec();
    m_szpp->wait();

    progress.close();
    progress.remove();

    qInfo() << "Extraction completed - Exit status:" << m_sevenz.exitStatus() 
            << "Exit code:" << m_sevenz.exitCode();

    return (m_sevenz.exitStatus() == QProcess::NormalExit) &&
           (0 == m_sevenz.exitCode());
}

bool SevenZip::check()
{
    qDebug() << "Starting archive verification:" << m_archiveFile;
    m_sevenz.setProgram(m_sevenZip);

    QStringList args;
    args << "t" << m_archiveFile;

    m_sevenz.setArguments(args);
    m_sevenz.start();
    if (m_sevenz.waitForStarted(-1) && m_sevenz.processId()) {
#ifdef Q_OS_LINUX
        QProcess::execute(QString("ionice -c3 -p %1").arg(m_sevenz.processId()));
#endif
        m_eventLoop.exec();
    }
    qInfo() << "Archive verification completed - Exit status:" << m_sevenz.exitStatus() 
            << "Exit code:" << m_sevenz.exitCode();
    return (m_sevenz.exitStatus() == QProcess::NormalExit) &&
           (0 == m_sevenz.exitCode());
}

void SevenZip::handleFinished()
{
    qDebug() << "7-Zip process finished";
    m_eventLoop.quit();
}

SevenZipProcessParser::SevenZipProcessParser(const QString &file, QProcess *process, QObject *parent): QThread(parent)
{
    qDebug() << "Initializing SevenZip process parser";
    m_progressFilename = file;
    m_sevenZip = process;
}

void SevenZipProcessParser::run()
{
    qDebug() << "Starting progress parsing";
    QFile progress(m_progressFilename);
    progress.open(QIODevice::ReadOnly);
    while (QProcess::NotRunning != m_sevenZip->state()) {
        QByteArray readed = progress.readLine();
        QString progressStr = QString::fromUtf8(readed).split("\b").last();
        QStringList prgressInfoList = progressStr.split(" - ");
        if (1 <= prgressInfoList.size()) {
            int pencent = prgressInfoList.first().split("% ").first().remove(" ").toInt();
            m_lastPencent = (pencent >= m_lastPencent) ? pencent : m_lastPencent;
        }
        if (2 <= prgressInfoList.size()) {
            m_lastFilename = prgressInfoList.last().isEmpty() ? m_lastFilename : prgressInfoList.last();
        }
        qDebug() << "Extraction progress:" << m_lastPencent << "% - Current file:" << m_lastFilename;
        emit progressChanged(m_lastPencent, 100, m_lastFilename);
        QThread::sleep(1);
    }
    qInfo() << "Progress parsing completed - Process state:" << m_sevenZip->state();
    progress.close();
}
