#ifndef DISKUNITY_H
#define DISKUNITY_H

#include <QObject>

class DiskUnity : public QObject
{
    Q_OBJECT
public:
    explicit DiskUnity(QObject *parent = 0);

    QString FormatDisk(const QString &diskDev);

    /*
        FixUsbDisk will recreate primary table of the usb disk
        and create only one primary partition as fat32 format,
        if the usb stick has muti partition, all partition will be
        delete!!!
    */
    bool FixUsbDisk(const QString& targetDev);

    /*
        GetPartitionDiskDev will return the
    */
    static int GetPartitionDiskDev(QString targetDev);
signals:

public slots:

};

#endif // DISKUNITY_H
