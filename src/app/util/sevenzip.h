#ifndef SEVENZIP_H
#define SEVENZIP_H

#include <QObject>

class SevenZip : public QObject
{
    Q_OBJECT
public:
    explicit SevenZip(QObject *parent = 0);

    void setArchiveFile(const QString &archiveFile);
    void setOutputDirectory(const QString &outputDir);

signals:
    void progressChanged(int);

public slots:
//    void run();

private:
    QString m_archiveFile;
    QString m_outputDir;
};

#endif // SEVENZIP_HHH
