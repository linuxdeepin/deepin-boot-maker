#pragma once
#include <QObject>
#include <QString>

namespace Utils  {
    QString UsbShowText(const QString &targetdev);
    void ClearTargetDev(const QString &targetPath);
    QStringList ListUsbDrives();
    bool CheckInstallDisk(const QString& targetDev);
}

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
