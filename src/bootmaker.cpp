
#include "unetbootin.h"
#include "bootmaker.h"

#include <QThread>
#include <QWidget>

BootMaker::BootMaker(QObject* parent): QObject(parent){
     unetbootinPtr = new unetbootin;
     unetbootinPtr->ubninitialize();
}


QStringList BootMaker::listUsbDrives(){
    QStringList usbDriverlist;
    usbDriverlist = unetbootinPtr->listcurdrives();
    return usbDriverlist;
}

QString BootMaker::url2LocalFile(QString url){
    return QUrl(url).toLocalFile();
}

int BootMaker::start(QString isoPath, QString usbDriver, bool biosMode) {
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

int BootMaker::processRate() {
    return unetbootinPtr->tprogress->rate();
}

bool BootMaker::isFinish() {
    return unetbootinPtr->isFinsh();
}

bool BootMaker::isISOImage(QString isoPath) {
    QFileInfo fileinfo(isoPath);
    if (fileinfo.suffix() == "iso")
        return true;
    QMessageBox msg;
    msg.setIcon(QMessageBox::Information);
    msg.setWindowTitle(tr("Please select a iso image"));
    msg.setText(tr("Please select a iso image"));
    msg.setStandardButtons(QMessageBox::Ok);
    msg.exec();
    return false;
}

void BootMaker::reboot() {
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

void BootMaker::exitRestart() {
    reboot();
}

QString BootMaker::homeDir() {
    qDebug()<<*(QStandardPaths::standardLocations(QStandardPaths::HomeLocation).begin())<<endl;
    return *(QStandardPaths::standardLocations(QStandardPaths::HomeLocation).begin());
}
