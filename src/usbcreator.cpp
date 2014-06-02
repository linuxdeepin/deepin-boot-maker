
#include "unetbootin.h"
#include "usbcreator.h"

#include <QThread>
#include <QWidget>

UsbCreator::UsbCreator(QObject* parent): QObject(parent){
     unetbootinPtr = new unetbootin;
     unetbootinPtr->ubninitialize();
}


QStringList UsbCreator::listUsbDrives(){
    QStringList usbDriverlist;
    usbDriverlist = unetbootinPtr->listcurdrives();
    return usbDriverlist;
}

QString UsbCreator::url2LocalFile(QString url){
    return QUrl(url).toLocalFile();
}

int UsbCreator::start(QString isoPath, QString usbDriver, bool biosMode) {
    unetbootinPtr->isoImagePath = isoPath;
    unetbootinPtr->usbDriverPath = usbDriver;
    unetbootinPtr->biosMode = biosMode;
    if (unetbootinPtr->checkInstallPara()){
        QThread *pwork = new QThread();
        unetbootinPtr->moveToThread(pwork);
        pwork->start();
        unetbootinPtr->start();
        return 0;
    }
    //失败
    return 1;
}

int UsbCreator::processRate() {
    return unetbootinPtr->tprogress->rate();
}

bool UsbCreator::isFinish() {
    return unetbootinPtr->isFinsh();
}

bool UsbCreator::isISOImage(QString isoPath) {
    if (!isoPath.isEmpty())
        return true;
    return false;
}
