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
