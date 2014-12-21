#ifndef USBINSTALLER_H
#define USBINSTALLER_H

#include "progressrate.h"
#include <QObject>

class QTimer;
class FileListMonitor;

class UsbInstaller : public QObject {
    Q_OBJECT
public:
    explicit UsbInstaller(QObject *parent = 0);

    FileListMonitor *fileMonitor;
    ProcessRate *progress;
    QString installPath;
    bool isFinsh_;
signals:
    void listUsbDevice(const QStringList& devicelist);

public slots:
    void start();
    bool installUSB(const QString &isopath, const QString &devpath, bool format);
    bool extractISO(const QString &isopath, const QString installdir);
    void finishInstall();

public:
    bool isFinsh();

protected slots:
    void listUsbDrives();

protected:
    QTimer *m_refreshTimer;
};


#endif // USBINSTALLER_H
