#ifndef SEVENZIP_H
#define SEVENZIP_H

#include <QObject>
#include <QThread>

class QProcess;

class SevenZip : public QThread
{
    Q_OBJECT
public:
    explicit SevenZip(QObject *parent = 0);

    void setArchiveFile(const QString &archiveFile);
    void setOutputDirectory(const QString &outputDir);

signals:
    void progressChanged(int);

public slots:
    void run() Q_DECL_OVERRIDE;

private:
    QString m_sevenZip;
    QString m_archiveFile;
    QString m_outputDir;
};

class SevenZipProcessParser : public QThread
{
    Q_OBJECT
public:
    explicit SevenZipProcessParser(const QString &file, QProcess* process, QObject *parent = 0);

    void run() Q_DECL_OVERRIDE;

signals:
    void progressChanged(int);

private:
    QString     m_progressFilename;
    QProcess    *m_sevenZip = nullptr;

    int         m_lastPencent = 0;
    QString     m_lastFilename;
};

#endif // SEVENZIP_HHH
