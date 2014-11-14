#ifndef USBCREATOR_H
#define USBCREATOR_H

#include <QString>
#include <QStringList>
#include <QWidget>

class unetbootin;
class FileListMonitor;
class ProcessRate;

class BootMaker : public QWidget{
   Q_OBJECT
public:
    explicit BootMaker (QWidget* parent = 0);
    //API
    Q_INVOKABLE QStringList listUsbDrives();
    Q_INVOKABLE int start(QString isoPath, QString usbDriver, bool formatDisk = false);
    Q_INVOKABLE int processRate();
    Q_INVOKABLE bool isFinish();
    Q_INVOKABLE bool isISOImage(QString isoPath);
    Q_INVOKABLE QString url2LocalFile(QString isoPath);
    Q_INVOKABLE void exitRestart();

    /*
        THE QML Messagebox is too sample, so do here
    */
    Q_INVOKABLE bool confirmFormatDlg();
    Q_INVOKABLE bool checkInstallPara();

public slots:
    void reboot();

signals:
    void process(const QString&, const QString&, bool);

protected:
    QString             m_ImagePath;
    QString             m_DriverPath;
    bool                m_FormatDisk;
    unetbootin          *m_UnetbootinPtr;
    FileListMonitor     *m_FileListMonitor;
    ProcessRate         *m_Progress;


};


const QString AppTitle = QObject::tr("Deepin Boot Maker");

#endif // USBCREATOR_H
