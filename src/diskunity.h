#ifndef DISKUNITY_H
#define DISKUNITY_H

#include <QObject>

class DiskUnity : public QObject
{
    Q_OBJECT
public:
    explicit DiskUnity(QObject *parent = 0);

    static QString InstallBootloader(const QString &diskDev);


    static bool ConfigSyslinx(const QString &targetDev);


    static bool EjectDisk(const QString &targetDev);

    /*
        GetPartitionDiskDev will return the
    */
    static QString GetPartitionDisk(const QString &targetDev);

protected:
    static bool UmountDisk(const QString &targetDev);
};

class FileListMonitor: public QObject {
    Q_OBJECT
public:
    FileListMonitor(QObject *parent = 0);
    qint64 FinishSize();

public slots:
    void ToNextFile(const QString &file);
    void SetTotalSize(qint64 size);

signals:
    void totalSize(qint64 size);
    void toNextFile(const QString &file);

private:
    qint64 finishSize_;
    qint64 totalSize_;
    QString currentFile_;
    //QStringList desList_;
};


#endif // DISKUNITY_H
