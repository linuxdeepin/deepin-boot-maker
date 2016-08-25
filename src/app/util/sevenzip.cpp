#include "sevenzip.h"

#include <QCoreApplication>
#include <QDebug>
#include <QProcess>
#include <QTemporaryFile>
#include <QThread>

SevenZip::SevenZip(QObject *parent) : QThread(parent)
{
    m_sevenZip = "7z";
    m_archiveFile = "/home/iceyer/Development/iso/atm-hengyin-amd64-2016-06-27.iso";
    m_outputDir = "-o/media/iceyer/C817-1EF7";
}

void SevenZip::run()
{
    QProcess sevenz;
    sevenz.setProgram(m_sevenZip);

    QTemporaryFile progress;
    progress.open();
    qDebug() << progress.fileName();

    QStringList args;
    args << "x" << "-y"
         << m_archiveFile
         << m_outputDir
         << "-bsp2";
    sevenz.setArguments(args);
    sevenz.setStandardErrorFile(progress.fileName());

    qDebug() << sevenz.program() << sevenz.arguments().join(" ");
    sevenz.start();
    sevenz.waitForStarted(-1);

    SevenZipProcessParser szpp(progress.fileName(), &sevenz);
    connect(&szpp, &SevenZipProcessParser::progressChanged, this, &SevenZip::progressChanged);
    szpp.start();

    sevenz.waitForFinished(-1);
    szpp.wait();

    progress.close();
    progress.remove();
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
        qDebug() << m_lastPencent << m_lastFilename;
        QThread::sleep(1);
        emit progressChanged(m_lastPencent);
    }
    qDebug() << "End Parse" << m_sevenZip->state();
    progress.close();
}
