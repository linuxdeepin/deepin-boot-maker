#ifndef BOOTMAKER_H
#define BOOTMAKER_H

#include <QObject>

class BootMaker : public QObject
{
    Q_OBJECT
public:
    explicit BootMaker(QObject *parent = 0);

signals:

public slots:
//    int blobsInstall();
//    int diskFormat();
//    int isoExtract();
//    int bootloaderInstall();
};

#endif // BOOTMAKER_H
