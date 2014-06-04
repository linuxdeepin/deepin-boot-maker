
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

void UsbCreator::reboot() {
#ifdef Q_OS_WIN32
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
    ExitWindowsEx(EWX_REBOOT, EWX_FORCE);
#endif
#ifdef Q_OS_LINUX
    unetbootinPtr->callexternapp("init", "6 &");
#endif
#ifdef Q_OS_MAC
    unetbootinPtr->callexternapp("shutdown", "-r now &");
#endif
}

void UsbCreator::exitRestart() {
    reboot();
}
