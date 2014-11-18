
#include "unetbootin.h"
#include "bootmaker.h"

#include "xsys.h"

#include "diskunity.h"

#include <QThread>

#ifdef Q_OS_WIN32
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "advapi32.lib")
#endif

const QString AppTitle() {
    return QObject::tr("Deepin Boot Maker");
}

BootMaker::BootMaker(QWidget* parent): QWidget(parent){
     m_UnetbootinPtr = new unetbootin();
     m_FileListMonitor = new FileListMonitor();
     m_Progress = new ProcessRate();
     m_UnetbootinPtr->ubninitialize();
     m_UnetbootinPtr->flm = m_FileListMonitor;
     m_UnetbootinPtr->tprogress = m_Progress;
}

QStringList BootMaker::listUsbDrives(){
    QStringList usbDriverlist;
    usbDriverlist = m_UnetbootinPtr->listcurdrives();
    return usbDriverlist;
}

QString BootMaker::url2LocalFile(QString url){
    return QUrl(url).toLocalFile();
}

int BootMaker::start(QString isoPath, QString usbDriver, bool formatDisk) {
    m_ImagePath = isoPath;
    m_DriverPath = usbDriver;
    m_FormatDisk = formatDisk;

    if (this->checkInstallPara()){

        if ((!formatDisk) && (!DiskUnity::CheckInstallDisk(usbDriver))){

            QMessageBox msgbox;
            msgbox.setIcon(QMessageBox::Critical);
            msgbox.setWindowTitle(tr("Format error of USB flash drive"));
            msgbox.setText(tr("Only FAT32 USB flash drive supported. Need to format? All partitions and data will be lost during formatting, please back up the data in advance."));
            msgbox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgbox.setButtonText(QMessageBox::Ok, tr("Format"));
            msgbox.setButtonText(QMessageBox::Cancel, tr("Cancel"));

            if(msgbox.exec() == QMessageBox::Ok) {
                 m_FormatDisk = true;
            } else {
                return 1;
            }
        }
        qDebug()<<"Start make boot disk";

        QThread *pwork = new QThread();
        m_UnetbootinPtr->moveToThread(pwork);
        QObject::connect(this, SIGNAL(process(QString, QString, bool)),
                         m_UnetbootinPtr, SLOT(startProcess(QString, QString, bool)));
        pwork->start();

        emit this->process(m_ImagePath, m_DriverPath, m_FormatDisk);
        qDebug()<<"BootMaker Return";
        return 0;
    }
    return 1;
}
#include <memory>

int BootMaker::processRate() {
    if (!(m_UnetbootinPtr->isFinsh_)) {
        m_Progress->setValue(m_FileListMonitor->FinishSize());
    } else {
        m_Progress->setValue(m_Progress->maximum());
    }
    qDebug()<<QString("processRate value: %1/total: %2, rate: %3").arg(m_Progress->value()).arg(m_Progress->maximum()).arg(m_Progress->rate());
    return m_Progress->rate();
}

bool BootMaker::isFinish() {
    return m_UnetbootinPtr->isFinsh();
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

bool BootMaker::confirmFormatDlg() {
    QMessageBox msgbox(this);
    msgbox.setIcon(QMessageBox::Critical);
    msgbox.setWindowTitle(tr("Format USB flash drive"));
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
    m_UnetbootinPtr->callexternapp("init", "6 &");
#endif
#ifdef Q_OS_MAC
    unetbootinPtr->callexternapp("shutdown", "-r now &");
#endif
}

void BootMaker::exitRestart() {
    reboot();
}


bool BootMaker::checkInstallPara() {
    if (m_DriverPath.isEmpty())
    {
        QMessageBox unotenoughinputmsgb(this);
        unotenoughinputmsgb.setIcon(QMessageBox::Information);
        unotenoughinputmsgb.setWindowTitle(tr("Insert a USB flash drive"));
        unotenoughinputmsgb.setText(tr("No USB flash drives were found. If you have already inserted a USB flash drive, try to reformat it as FAT32."));
        unotenoughinputmsgb.setStandardButtons(QMessageBox::Ok);
        unotenoughinputmsgb.setButtonText(QMessageBox::Ok, tr("Ok"));
        switch (unotenoughinputmsgb.exec())
        {
            case QMessageBox::Ok:
                break;
            default:
                break;
        }
    }
#ifdef Q_OS_MAC
    if (locatemountpoint(m_DriverPath) == "NOT MOUNTED")
        callexternapp("diskutil", "mount "+usbDriverPath);
#endif
    #ifdef Q_OS_LINUX
    else if (m_UnetbootinPtr->locatemountpoint(m_DriverPath) == "NOT MOUNTED")
    {
        QMessageBox merrordevnotmountedmsgbx(this);
        merrordevnotmountedmsgbx.setIcon(QMessageBox::Warning);
        merrordevnotmountedmsgbx.setWindowTitle(QString(tr("%1 not mounted")).arg(m_DriverPath));
        merrordevnotmountedmsgbx.setText(QString(tr("You must firstly mount the USB flash drive %1 to a mountpoint. Most distributions will do this automatically after you remove and reinsert the USB flash drive.")).arg(m_DriverPath));
        merrordevnotmountedmsgbx.setStandardButtons(QMessageBox::Ok);
        merrordevnotmountedmsgbx.setButtonText(QMessageBox::Ok, tr("Ok"));
        switch (merrordevnotmountedmsgbx.exec())
        {
            case QMessageBox::Ok:
                break;
            default:
                break;
        }
    }
    #endif
    else if (m_ImagePath.isEmpty())
    {
        QMessageBox fnotenoughinputmsgb(this);
        fnotenoughinputmsgb.setIcon(QMessageBox::Information);
        fnotenoughinputmsgb.setWindowTitle(tr("Select a disk image file"));
        fnotenoughinputmsgb.setText(tr("You must select a disk image file to load."));
        fnotenoughinputmsgb.setStandardButtons(QMessageBox::Ok);
        fnotenoughinputmsgb.setButtonText(QMessageBox::Ok, tr("Ok"));
        switch (fnotenoughinputmsgb.exec())
        {
            case QMessageBox::Ok:
                break;
            default:
                break;
        }
    }
    else if (!QFile::exists(m_ImagePath) && !m_ImagePath.startsWith("http://") && !m_ImagePath.startsWith("ftp://"))
    {
        QMessageBox ffnotexistsmsgb(this);
        ffnotexistsmsgb.setIcon(QMessageBox::Information);
        ffnotexistsmsgb.setWindowTitle(tr("Disk image file not found"));
        ffnotexistsmsgb.setText(tr("The specified disk image file %1 does not exist.").arg(m_ImagePath));
        ffnotexistsmsgb.setStandardButtons(QMessageBox::Ok);
        ffnotexistsmsgb.setButtonText(QMessageBox::Ok, tr("Ok"));
        switch (ffnotexistsmsgb.exec())
        {
            case QMessageBox::Ok:
                break;
            default:
                break;
        }
    }
    else
    {
        return true;
    }
    return false;
}
