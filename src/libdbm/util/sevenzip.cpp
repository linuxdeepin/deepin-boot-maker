// SPDX-FileCopyrightText: 2017 - 2026 UnionTech Software Technology Co., Ltd.
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
    //    connect(&m_szpp, &SevenZipProcessParser::progressChanged, this, &SevenZip::progressChanged);
        connect(&m_sevenz, static_cast<void(QProcess::*)(int exitCode)>(&QProcess::finished),
                this, &SevenZip::handleFinished);
}

void SevenZip::setArchiveFile(const QString &archiveFile)
{
    m_archiveFile = archiveFile;
}

void SevenZip::setOutputDirectory(const QString &outputDir)
{
    m_outputDir = "-o" + outputDir;
}

void SevenZip::stopProcess()
{
    QString strCmd = QString("kill 9 %1").arg(m_sevenz.processId());
    qDebug() << strCmd;
    QProcess::execute(strCmd);
}

QStringList SevenZip::parseOversizeBootPaths(const QString &sltOutput)
{
    // FAT32 single file size limit: 4GiB - 1 byte.
    static const qint64 kFat32MaxFileSize = 4294967295LL;

    QStringList result;

    QString curPath;
    bool curIsFile = false;
    bool curSizeValid = false;
    qint64 curSize = 0;

    // An entry ends when the next "Path = " starts, so we only rely on fields
    // that exist in both old p7zip and new 7-Zip `-slt` output.
    auto flush = [&]() {
        if (curIsFile && curSizeValid && curSize > kFat32MaxFileSize
                && curPath.startsWith(QStringLiteral("[BOOT]/"))) {
            result << curPath;
        }
        curPath.clear();
        curIsFile = false;
        curSizeValid = false;
        curSize = 0;
    };

    const QStringList lines = sltOutput.split(QLatin1Char('\n'));
    for (QString line : lines) {
        if (line.endsWith(QLatin1Char('\r')))
            line.chop(1);

        if (line.startsWith(QStringLiteral("Path = "))) {
            flush();
            curPath = line.mid(7);
        } else if (line.startsWith(QStringLiteral("Folder = "))) {
            curIsFile = (line.mid(9).trimmed() == QLatin1String("-"));
        } else if (line.startsWith(QStringLiteral("Size = "))) {
            const QString sizeStr = line.mid(7).trimmed();
            curSize = sizeStr.toLongLong(&curSizeValid);
        }
    }
    flush();

    return result;
}

QStringList SevenZip::oversizeBootPaths() const
{
    QStringList result;

    QProcess listProcess;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LANG", "zh_CN.UTF-8");
    listProcess.setProcessEnvironment(env);

    qInfo() << "scan iso for oversize [BOOT] entries:" << m_sevenZip << m_archiveFile;
    listProcess.start(m_sevenZip, QStringList() << "l" << "-slt" << m_archiveFile);
    if (!listProcess.waitForStarted(-1) || !listProcess.waitForFinished(-1)) {
        // Fall back to no exclusion so old/unsupported 7z keeps working.
        qWarning() << "scan iso failed, skip [BOOT] exclusion:" << listProcess.errorString();
        return result;
    }

    result = parseOversizeBootPaths(QString::fromUtf8(listProcess.readAllStandardOutput()));
    if (!result.isEmpty())
        qInfo() << "detected oversize [BOOT] entries:" << result;
    return result;
}

bool SevenZip::extract()
{
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

    // Newer 7-Zip versions expose the El Torito boot image as a synthetic
    // "[BOOT]/..." entry whose data overlaps the live filesystem. When such an
    // entry exceeds the FAT32 single file limit it cannot be written to the
    // target partition and extraction fails. Exclude only the oversized
    // entries; versions that do not expose them (e.g. old p7zip) return an
    // empty list and keep the previous behavior.
    const QStringList oversizeBootEntries = oversizeBootPaths();
    for (const QString &bootPath : oversizeBootEntries) {
        qWarning() << "exclude oversize [BOOT] entry from extraction:" << bootPath;
        args << (QStringLiteral("-x!") + bootPath);
    }

    QStringList env = QProcess::systemEnvironment();
    env << "VDPAU_DRIVER=va_gl";

    m_sevenz.setArguments(args);
    m_sevenz.setStandardErrorFile(progress.fileName());

    qDebug() << m_sevenz.program() << m_sevenz.arguments().join(" ");
    m_sevenz.start();
    m_sevenz.waitForStarted(-1);

#ifdef Q_OS_LINUX
    QProcess::execute(QString("ionice -c3 -p %1").arg(m_sevenz.pid()));
#endif

    m_szpp->setProgressName(progress.fileName());
    m_szpp->start();
    m_eventLoop.exec();
    m_szpp->wait();

    progress.close();
    progress.remove();

    qInfo() << m_sevenz.exitStatus() << m_sevenz.exitCode();

    return (m_sevenz.exitStatus() == QProcess::NormalExit) &&
           (0 == m_sevenz.exitCode());
}

bool SevenZip::check()
{
    m_sevenz.setProgram(m_sevenZip);

    QStringList args;
    args << "t" << m_archiveFile;

    m_sevenz.setArguments(args);
    m_sevenz.start();
    if (m_sevenz.waitForStarted(-1) && m_sevenz.pid()) {
        #ifdef Q_OS_LINUX
            QProcess::execute(QString("ionice -c3 -p %1").arg(m_sevenz.pid()));
        #endif
        m_eventLoop.exec();
    }
    qInfo() << "check iso result" << m_sevenz.exitStatus() << m_sevenz.exitCode();
    return (m_sevenz.exitStatus() == QProcess::NormalExit) &&
           (0 == m_sevenz.exitCode());
}

void SevenZip::handleFinished()
{
    qDebug() <<"Zip Event Exit";
    m_eventLoop.quit();
}

SevenZipProcessParser::SevenZipProcessParser(const QString &file, QProcess *process, QObject *parent): QThread(parent)
{
    m_progressFilename = file;
    m_sevenZip = process;
}

void SevenZipProcessParser::run()
{
    qDebug() << "Start Parse";
    QFile progress(m_progressFilename);
//    qDebug() << "progressFilename:" << m_progressFilename;
    progress.open(QIODevice::ReadOnly);
    while (QProcess::NotRunning != m_sevenZip->state()) {
        QByteArray  readed = progress.readLine();
        QString progressStr = QString::fromUtf8(readed).split("\b").last();
        QStringList prgressInfoList =  progressStr.split(" - ");
        if (1 <= prgressInfoList.size()) {
            int pencent = prgressInfoList.first().split("% ").first().remove(" ").toInt();
            m_lastPencent = (pencent >= m_lastPencent) ? pencent : m_lastPencent;
        }
        if (2 <= prgressInfoList.size()) {
            m_lastFilename = prgressInfoList.last().isEmpty() ? m_lastFilename : prgressInfoList.last();
        }
        qInfo() << "send SevenZip progress" << m_lastPencent << m_lastFilename;
        emit progressChanged(m_lastPencent, 100, m_lastFilename);
        QThread::sleep(1);
    }
    qInfo() << "End Parse" << m_sevenZip->state();
    progress.close();
}
