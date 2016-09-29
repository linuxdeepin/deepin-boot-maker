#ifndef BOOTMAKER_H
#define BOOTMAKER_H

#include <QObject>

class BootMakerBackendDaemon;
class UsbDeviceMonitor;

class BootMaker : public QObject
{
    Q_OBJECT
public:
    explicit BootMaker(QObject *parent = 0);

signals:
    void finished(int errcode, const QString &description);
    void reportProgress(int current, int total, const QString &title, const QString &description);


public slots:
    bool install(const QString &image,
                 const QString &device,
                 const QString &partition,
                 bool  formatDevice);

private:
    BootMakerBackendDaemon *m_porgressReporter = nullptr;
    UsbDeviceMonitor *m_usbDeviceMonitor = nullptr;

};

#endif // BOOTMAKER_H
