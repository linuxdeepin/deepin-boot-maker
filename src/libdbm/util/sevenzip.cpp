#include "sevenzip.h"

#include <QCoreApplication>
#include <QDebug>
#include <QProcess>
#include <QTemporaryFile>
#include <QThread>

#include <XSys>

SevenZip::SevenZip(const QString &image, const QString &target, QObject *parent)
    : QObject(parent)
{
#ifdef Q_OS_WIN32
    QString sevnz = XSys::FS::InsertTmpFile(":/blob/sevnz/sevnz.exe");
    QString sevnzdll = XSys::FS::InsertTmpFile(":/blob/sevnz/sevnz.dll");
    qDebug() << sevnz << sevnzdll;
#endif
#ifdef Q_OS_MAC
    QDir resourceDir = QDir(QApplication::applicationDirPath());
    resourceDir.cdUp();
    resourceDir.cd("Resources");
    QString sevnz = resourceDir.absoluteFilePath("7z-mac");
#endif
#ifdef Q_OS_LINUX
    QString sevnz = "7z";
#endif

    m_szpp = new SevenZipProcessParser("", &m_sevenz);
    m_sevenZip = sevnz;
    m_archiveFile = image;
    m_outputDir = "-o" + target;
    //    connect(&m_szpp, &SevenZipProcessParser::progressChanged, this, &SevenZip::progressChanged);
    //    connect(&m_sevenz, static_cast<void(QProcess::*)(int exitCode)>(&QProcess::finished),
    //            this, &SevenZip::handleFinished);
}

bool SevenZip::extract()
{
    m_sevenz.setProgram(m_sevenZip);

    QTemporaryFile progress;
    progress.open();
    qDebug() << progress.fileName();

    QStringList args;
    args << "x" << "-y"
         << m_archiveFile
         << m_outputDir
         << "-bsp2";

    QStringList env = QProcess::systemEnvironment();
    env << "VDPAU_DRIVER=va_gl";

    m_sevenz.setArguments(args);
    m_sevenz.setStandardErrorFile(progress.fileName());

    qDebug() << m_sevenz.program() << m_sevenz.arguments().join(" ");
    m_sevenz.start();
    m_sevenz.waitForStarted(-1);

    m_szpp->setProgressName(progress.fileName());
    m_szpp->start();
    m_sevenz.waitForFinished(-1);
    m_szpp->wait();

    progress.close();
    progress.remove();

    qDebug() << m_sevenz.exitStatus() << m_sevenz.exitCode();

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
    m_sevenz.waitForStarted(-1);
    m_sevenz.waitForFinished(-1);

    qDebug() << "check iso result" << m_sevenz.exitStatus() << m_sevenz.exitCode();

    return (m_sevenz.exitStatus() == QProcess::NormalExit) &&
           (0 == m_sevenz.exitCode());
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
        qDebug() << "send SevenZip progress" << m_lastPencent << m_lastFilename;
        emit progressChanged(m_lastPencent, 100, m_lastFilename);
        QThread::sleep(1);
    }
    qDebug() << "End Parse" << m_sevenZip->state();
    progress.close();
}
