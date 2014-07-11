
#include "unetbootin.h"
#include "bootmaker.h"

#include "xsys.h"

#include "diskunity.h"

#include <QThread>
#include <QWidget>


BootMaker::BootMaker(QObject* parent): QObject(parent){
     unetbootinPtr = new unetbootin;
     flm = new FileListMonitor();
     tprogress = new ProcessRate();
     unetbootinPtr->ubninitialize();
     unetbootinPtr->flm = flm;
     unetbootinPtr->tprogress = tprogress;
}


QStringList BootMaker::listUsbDrives(){
    QStringList usbDriverlist;
    usbDriverlist = unetbootinPtr->listcurdrives();
    return usbDriverlist;
}

QString BootMaker::url2LocalFile(QString url){
    return QUrl(url).toLocalFile();
}

int BootMaker::start(QString isoPath, QString usbDriver, bool biosMode, bool formatDisk) {

    unetbootinPtr->isoImagePath = isoPath;
    unetbootinPtr->usbDriverPath = usbDriver;
    unetbootinPtr->biosMode = biosMode;
    unetbootinPtr->formatDisk = formatDisk;
    if (unetbootinPtr->checkInstallPara()){

        QThread *pwork = new QThread();
        unetbootinPtr->moveToThread(pwork);
        pwork->start();
        unetbootinPtr->start();
        return 0;
    }
    return 1;
}
#include <memory>

int BootMaker::processRate() {
    if (!unetbootinPtr->isFinsh_) {
        tprogress->setValue(flm->FinishSize());
    } else {
        tprogress->setValue(tprogress->maximum());
    }
    qDebug()<<QString("value: %1/total: %2, rate: %3").arg(tprogress->value()).arg(tprogress->maximum()).arg(tprogress->rate());
    return tprogress->rate();
}

bool BootMaker::isFinish() {
     qDebug()<<"isFinish: "<<unetbootinPtr->isFinsh();
    return unetbootinPtr->isFinsh();
}

bool BootMaker::isISOImage(QString isoPath) {
    QFileInfo fileinfo(isoPath);
    if (fileinfo.suffix() == "iso")
        return true;
    QMessageBox msg;
    msg.setIcon(QMessageBox::Information);
    msg.setWindowTitle(tr("Please select an iso image"));
    msg.setText(tr("Please select an iso image"));
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
