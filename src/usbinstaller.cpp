#include "usbinstaller.h"

#include "xsys.h"
#include "diskunity.h"

#include <QStringList>
#include <QDebug>
#include <QFileInfo>
#include <QtWidgets/QApplication>
#include <QDir>
#include <QTimer>
#include <QStringList>

UsbInstaller::UsbInstaller(QObject *parent) :
    QObject(parent) {
    isFinsh_ = false;

}

void UsbInstaller::start() {
    m_refreshTimer = new QTimer();
    m_refreshTimer->setInterval(1000);
    connect(m_refreshTimer, SIGNAL(timeout()),
            this, SLOT(listUsbDrives()));
    m_refreshTimer->start();
}

void UsbInstaller::listUsbDrives() {
    m_refreshTimer->setInterval(8000);
    QStringList list = ListUsbDrives();
    emit listUsbDevice(list);
}

void UsbInstaller::finishInstall() {
    progress->setValue(this->progress->maximum() * 98 / 100);
    qDebug() << (tr("Installing syslinux to %1").arg(installPath));

    DiskUnity::ConfigSyslinx(installPath);

#ifdef Q_OS_UNIX
    progress->setValue(progress->maximum() * 99 / 100);
    qDebug() << ("Syncing filesystems");
    XSys::SynExec("sync", "");
#endif

    progress->setValue(progress->maximum());

    isFinsh_ = true;
    DiskUnity::EjectDisk(installPath);
}

bool UsbInstaller::isFinsh() {
    return isFinsh_;
}

struct ArchiveFileInfo {
    QString Path;
    qint64  Size;
};

bool UsbInstaller::extractISO(const QString& isopath, const QString installdir) {
#ifdef Q_OS_WIN32
    QString sevnz = XSys::InsertTmpFile(QString(":/bolbs/sevnz/sevnz.exe"));
    QString sevnzdll = XSys::InsertTmpFile(QString(":/bolbs/sevnz/sevnz.dll"));
    QFileInfo szinfo (sevnzdll);
    QFile  szdll(sevnzdll);
    szdll.rename(szinfo.dir().absolutePath() + "/7z.dll");
#endif
#ifdef Q_OS_MAC
    QDir resourceDir = QDir(QApplication::applicationDirPath());
    resourceDir.cdUp();
    resourceDir.cd("Resources");
    QString sevnz = resourceDir.absoluteFilePath("7z-mac");
#endif
#ifdef Q_OS_LINUX
    QString sevnz = "7z";
#endif

    QString sevnzout = XSys::SynExec(sevnz, QString("-bd -slt l \"%1\"").arg(QFileInfo(isopath).absoluteFilePath()));

    QList<ArchiveFileInfo> arfilelist;
#ifdef Q_OS_WIN32
    QStringList itemlist = sevnzout.split("\r\n\r\n").filter("Folder = -");
#else
    QStringList itemlist = sevnzout.split("\n\n").filter("Folder = -");
#endif

    QStringList::iterator itor = itemlist.begin();

    for(; itor != itemlist.end(); ++itor) {
        QStringList infolist = (*itor).split("\n");
        QStringList::iterator infoitor = infolist.begin();
        ArchiveFileInfo arinfo;

        for(; infoitor != infolist.end(); ++ infoitor) {
            if((*infoitor).contains("Packed")) {
                continue;
            }

            if((*infoitor).contains("Size")) {
                arinfo.Size = (*infoitor).remove("Size = ").toLong();
                continue;
            }

            if((*infoitor).contains("Path = ")) {
                arinfo.Path = (*infoitor).remove("Path = ").remove("\r").remove("\n");
                continue;
            }
        }

        arfilelist.push_back(arinfo);
    }

    QList<ArchiveFileInfo>::iterator arfileitor = arfilelist.begin();

//    for(; arfileitor != arfilelist.end(); ++arfileitor) {
//        if (arfileitor->Path) {
//            arfilelist.erase(arfileitor);
//        }
//    }

    //extract file in arfilelist
    QFileInfo isoinfo(isopath);
    fileMonitor->SetTotalSize(isoinfo.size(), arfilelist.size());
    qint64 ts = fileMonitor->getTotalSize();
    progress->setMaximum(ts  * 100 / 98);
    progress->setMinimum(0);
    progress->setValue(0);

    qDebug() << "Archive file list size:"<<arfilelist.size();
    int i = 0;
    for(arfileitor = arfilelist.begin(); arfileitor != arfilelist.end(); ++arfileitor) {
        i++;
        qDebug() << (tr("Extracted: %1/%2 rate: %3").arg(i).arg(arfilelist.size())).arg(progress->rate());
        progress->setValue(fileMonitor->FinishSize());
        qDebug() << arfileitor->Size << "\t" << arfileitor->Path;
        fileMonitor->ToNextFile(installdir + "/" + arfileitor->Path);
        XSys::SynExec(sevnz, QString(" -bd -y -o\"%1\" x \"%2\"  \"%3\"").arg(installdir).arg(isopath).arg(arfileitor->Path));
    }

    return true;
}

bool UsbInstaller::installUSB(const QString& isopath, const QString &target, bool format) {
    m_refreshTimer->stop();
    QString targetDev = target;
    QString installDir = targetDev;
#ifdef Q_OS_WIN32
    targetDev = targetDev.remove("\\");
    installDir = targetDev + "\\";
#endif
    QString rawtargetDev = targetDev;
#ifdef Q_OS_LINUX

    if(targetDev.contains(QRegExp("p\\d$")))
        rawtargetDev = QString(targetDev).remove(QRegExp("p\\d$"));
    else
        rawtargetDev = QString(targetDev).remove(QRegExp("\\d$"));

#endif
#ifdef Q_OS_MAC
    rawtargetDev = QString(targetDev).remove(QRegExp("s\\d$"));
#endif

    //install bootloader
    if(format) {
        targetDev = DiskUnity::InstallBootloader(rawtargetDev);
    } else {
        DiskUnity::InstallSyslinux(targetDev);
    }

#ifdef Q_OS_UNIX
    installDir = XAPI::MountPoint(targetDev);
#endif

    if (installDir.isEmpty()) {
        return false;
    }
    // Extract ISO
    DiskUnity::ClearTargetDev(installDir);
    extractISO(isopath, installDir);

    this->installPath = installDir;
    this->finishInstall();
    return true;
}
