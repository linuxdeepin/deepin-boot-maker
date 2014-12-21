#ifndef DISKUNITY_H
#define DISKUNITY_H

#include <QObject>
#include <QString>

class DiskUnity : public QObject {
    Q_OBJECT
public:
    explicit DiskUnity(QObject *parent = 0);

    static void ClearTargetDev(const QString &targetPath);

    static QString InstallBootloader(const QString &diskDev);

    static bool InstallSyslinux(const QString &diskDev);

    static bool ConfigSyslinx(const QString &targetDev);

    static bool CheckInstallDisk(const QString &targetDev);

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
    qint64 getTotalSize() {
        return totalSize_;
    }

public slots:
    void ToNextFile(const QString &file);
    void SetTotalSize(qint64 size, qint64 tatalNum);

signals:
    void totalSize(qint64 size, qint64 tatalNum);
    void toNextFile(const QString &file);

private:
    qint64 finishSize_;
    qint64 totalSize_;
    QString currentFile_;
    qint64  fakeSizePerFile_;
    qint64  fileNum_;
    //QStringList desList_;
};

QStringList ListUsbDrives();


#ifdef Q_OS_UNIX
namespace XAPI {
const QString MountPoint(const QString &targetDev);
}
#endif

#endif // DISKUNITY_H
