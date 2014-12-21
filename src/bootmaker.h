#ifndef USBCREATOR_H
#define USBCREATOR_H

#include <QString>
#include <QStringList>
#include <QWidget>

class unetbootin;
class FileListMonitor;
class ProcessRate;
class UsbInstaller;

class BootMaker : public QWidget {
    Q_OBJECT
public:
    explicit BootMaker(QWidget* parent = 0);

    Q_INVOKABLE int start(QString isoPath, QString usbDriver, bool formatDisk = false);
    Q_INVOKABLE int processRate();
    Q_INVOKABLE bool isFinish();
    Q_INVOKABLE bool isISOImage(QString isoPath);
    Q_INVOKABLE void exitRestart();

    Q_INVOKABLE bool confirmFormatDlg();
    Q_INVOKABLE bool checkInstallPara();

public slots:
    void reboot();

signals:
    void process(const QString&, const QString&, bool);
    void listUsbDevice(const QStringList& devicelist);

    void start();

protected:
    QString             m_ImagePath;
    QString             m_DriverPath;
    bool                m_FormatDisk;
    FileListMonitor     *m_FileListMonitor;
    ProcessRate         *m_Progress;
    UsbInstaller        *m_Installer;


};


const QString AppTitle();

#endif // USBCREATOR_H
