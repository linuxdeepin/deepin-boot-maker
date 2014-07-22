
#include "unetbootin.h"
#include "bootmaker.h"

#include "xsys.h"

#include "diskunity.h"

#include <QThread>

BootMaker::BootMaker(QWidget* parent): QWidget(parent){
     unetbootinPtr = new unetbootin(this);
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

        if ((!formatDisk) && (!DiskUnity::CheckInstallDisk(usbDriver))){

            QMessageBox msgbox(this);
            msgbox.setIcon(QMessageBox::Critical);
            msgbox.setWindowTitle(tr("Format error of USB flash drive"));
            msgbox.setText(tr("Only FAT32 USB flash drive supported. Need to format? All partitions and data will be lost during formatting, please back up the data in advance."));
            msgbox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgbox.setButtonText(QMessageBox::Ok, tr("Format"));
            msgbox.setButtonText(QMessageBox::Cancel, tr("Cancel"));

            if(msgbox.exec() == QMessageBox::Ok) {
                 unetbootinPtr->formatDisk = true;
            } else {
                return 1;
            }
        }
        qDebug()<<"Start make boot disk";

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
    if (!(unetbootinPtr->isFinsh_)) {
        tprogress->setValue(flm->FinishSize());
    } else {
        tprogress->setValue(tprogress->maximum());
    }
    qDebug()<<QString("processRate value: %1/total: %2, rate: %3").arg(tprogress->value()).arg(tprogress->maximum()).arg(tprogress->rate());
    return tprogress->rate();
}

bool BootMaker::isFinish() {
    return unetbootinPtr->isFinsh();
}

bool BootMaker::isISOImage(QString isoPath) {
    QFileInfo fileinfo(isoPath);
    if (fileinfo.suffix() == "iso")
        return true;
    QMessageBox msg(this);
    msg.setIcon(QMessageBox::Information);
    msg.setWindowTitle(tr("Please select an iso image"));
    msg.setText(tr("Please select an iso image"));
    msg.setStandardButtons(QMessageBox::Ok);
    msg.exec();
    return false;
}

bool BootMaker::confirmFormatDlg() {
    QMessageBox msgbox(this);
    msgbox.setIcon(QMessageBox::Critical);
    msgbox.setWindowTitle(tr("Format USB flash disk"));
    msgbox.setText(tr("All data will be lost during formatting, please back up in advance and then press OK button."));
    msgbox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgbox.setButtonText(QMessageBox::Ok, tr("Ok"));
    msgbox.setButtonText(QMessageBox::Cancel, tr("Cancel"));

    if(msgbox.exec() == QMessageBox::Ok) {
         return true;
    } else {
        return false;
    }
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
