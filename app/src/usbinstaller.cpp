#include "usbinstaller.h"

#include <XSys>

#include "utils.h"

#include <QStringList>
#include <QDebug>
#include <QFileInfo>
#include <QtWidgets/QApplication>
#include <QDir>
#include <QTimer>
#include <QStringList>

class Error {
public:
    enum ErrorType {
        SyscExecFailed,
        USBFormatError,
        USBSizeError,
        USBMountFailed,
        ExtractImgeFailed,
    };
    static const QString get(ErrorType et);
};

// TODO: Installation logs are stored in% 1, you can upload to forum to help us solve your problem.
const QString Error::get(const ErrorType et) {
    switch(et) {
    case SyscExecFailed:
        return QObject::tr("Failed to call the command %1.");
    case USBFormatError:
        return QObject::tr("Wrong USB flash drive format, please format to FAT32.");
    case USBSizeError:
        return QObject::tr("USB flash drive space is insufficient, ensure you have at least %1 free space.");
    case USBMountFailed:
        return QObject::tr("Failed to mount USB flash drive, please close other applications may use it and reinsert.");
    case ExtractImgeFailed:
        return QObject::tr("Failed to unzip the mirror file, please use the whole mirror file.");
    default:
        return "Internal Error";
    }
}

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
    QStringList list = Utils::ListUsbDrives();
    emit listUsbDevice(list);
}

void UsbInstaller::finishInstall() {
    progress->setValue(this->progress->maximum() * 98 / 100);
    qDebug() << (tr("Installing syslinux to %1").arg(installPath));

    XSys::Bootloader::Syslinux::ConfigSyslinx(installPath);

#ifdef Q_OS_UNIX
    progress->setValue(progress->maximum() * 99 / 100);
    qDebug() << ("Syncing filesystems");
    XSys::SynExec("sync", "");
#endif

    progress->setValue(progress->maximum());

    isFinsh_ = true;
    XSys::DiskUtil::EjectDisk(installPath);
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
    QString sevnz = XSys::FS::InsertTmpFile(":/blobs/sevnz/sevnz.exe");
    QString sevnzdll = XSys::FS::InsertTmpFile(":/blobs/sevnz/sevnz.dll");
    QFileInfo szinfo(sevnzdll);
    QFile  szdll(sevnzdll);
    szdll.rename(szinfo.dir().absolutePath() + "/7z.dll");
    qDebug()<<sevnzdll<<szinfo.dir().absolutePath();
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


    XSys::Result result = XSys::SynExec(sevnz, QString("-bd -slt l \"%1\"").arg(QFileInfo(isopath).absoluteFilePath()));

    if(!result.isSuccess()) {
        return false;
    }

    QString sevnzout = result.result();
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

    // TODO: make sure the iso is deepin iso
    if (arfilelist.isEmpty()) {
        qWarning()<<"Image file contains no files";
        return false;
    }

    QList<ArchiveFileInfo>::iterator arfileitor = arfilelist.begin();

    //extract file in arfilelist
    QFileInfo isoinfo(isopath);
    fileMonitor->SetTotalSize(isoinfo.size(), arfilelist.size());
    qint64 ts = fileMonitor->getTotalSize();
    progress->setMaximum(ts  * 100 / 98);
    progress->setMinimum(0);
    progress->setValue(0);

    qDebug() << "Archive file list size:" << arfilelist.size();
    int i = 0;

    for(arfileitor = arfilelist.begin(); arfileitor != arfilelist.end(); ++arfileitor) {
        i++;
        qDebug() << QString("Extracted: %1/%2 rate: %3").arg(i).arg(arfilelist.size()).arg(progress->rate());
        progress->setValue(fileMonitor->FinishSize());
        qDebug() << arfileitor->Size << "\t" << arfileitor->Path;
        fileMonitor->ToNextFile(installdir + "/" + arfileitor->Path);
        result = XSys::SynExec(sevnz, QString(" -bd -y -o\"%1\" x \"%2\"  \"%3\"").arg(installdir).arg(isopath).arg(arfileitor->Path));
        if(!result.isSuccess()) {
            //check size
            if (XSys::DiskUtil::GetPartitionFreeSpace(installdir) <= arfileitor->Size ) {
                qWarning()<<"DiskFull";
                throw Error::get(Error::USBSizeError);
                return false;
            }
            return false;
        }
    }

    return true;
}

bool UsbInstaller::installUSB(const QString& isopath, const QString &target, bool format) {
    m_refreshTimer->stop();
    qDebug()<<"UsbInstaller::installUSB start";
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
    XSys::Result result;
    if(format) {
        result = XSys::Bootloader::InstallBootloader(rawtargetDev);
        targetDev = result.result();
    } else {
        result = XSys::Bootloader::Syslinux::InstallSyslinux(targetDev);
    }

    if(! result.isSuccess()) {
        qWarning()<<result.errmsg();
        emit error(Error::get(Error::SyscExecFailed).arg(result.cmd()) + " " + result.errmsg());
        return false;
    }

#ifdef Q_OS_UNIX
    installDir = XSys::DiskUtil::MountPoint(targetDev);
#endif

    if(installDir.isEmpty()) {
        qWarning()<<"Error::get(Error::USBMountFailed)";
        emit error(Error::get(Error::USBMountFailed));
        return false;
    }

    // Extract ISO
    Utils::ClearTargetDev(installDir);

    try {
        if(!extractISO(isopath, installDir)) {
            qWarning()<<"Error::get(Error::ExtractImgeFailed)";
            emit error(Error::get(Error::ExtractImgeFailed));
            return false;
        }
    } catch(QString exception) {
        if (exception == Error::get(Error::USBSizeError)) {
            qWarning()<<exception;
            QFileInfo isoinfo(isopath);
            QString sizeM = QString("%1M").arg(isoinfo.size() / 1024 / 1024);
            emit error(Error::get(Error::USBSizeError).arg(sizeM));
            return false;
        }
        emit error(exception);
        return false;
    }

    this->installPath = installDir;
    this->finishInstall();
    emit finish();
    return true;
}
