#ifndef USBCREATOR_H
#define USBCREATOR_H

#include <QObject>
#include <QString>
#include <QStringList>

class unetbootin;

class BootMaker : public QObject{
   Q_OBJECT
public:
    explicit BootMaker (QObject* parent = 0);
    //对外API
    Q_INVOKABLE QStringList listUsbDrives();
    Q_INVOKABLE int start(QString isoPath, QString usbDriver, bool biosMode);
    Q_INVOKABLE int processRate();
    Q_INVOKABLE bool isFinish();
    Q_INVOKABLE bool isISOImage(QString isoPath);
    Q_INVOKABLE QString url2LocalFile(QString isoPath);
    Q_INVOKABLE void exitRestart();

    Q_INVOKABLE QString homeDir();

    unetbootin  *unetbootinPtr;
protected:
    void reboot();
};

#endif // USBCREATOR_H
