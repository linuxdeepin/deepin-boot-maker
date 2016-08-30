#include "bootmaker.h"

#include <QDebug>

#include <XSys>

#include "../util/sevenzip.h"
#include "../util/utils.h"
#include "../util/bmreporter.h"

#include "diskutil.h"

class Error
{
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
const QString Error::get(const ErrorType et)
{
    switch (et) {
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


BootMaker::BootMaker(QObject *parent) : QObject(parent)
{
    m_bmr = new BMReporter;
    QThread *reportWork = new QThread;
    m_bmr->moveToThread(reportWork);
    reportWork->start();

    connect(this, &BootMaker::reportProgress, m_bmr, &BMReporter::reportProgress);
}

bool BootMaker::install(const QString &image, const QString &device, const QString &partition, bool formatDevice)
{
    qDebug() << image << device << partition << formatDevice;

    this->reportProgress(1, 100, "install bootloader", "");

    QString targetPartition = partition;
    XSys::Result result;
    if (formatDevice) {
        result = XSys::Bootloader::InstallBootloader(device);
        targetPartition = result.result();
    } else {
        result = XSys::Bootloader::Syslinux::InstallSyslinux(partition);
    }

    if (! result.isSuccess()) {
        qCritical() << result.errmsg();
        emit finished(Error::SyscExecFailed, Error::get(Error::SyscExecFailed).arg(result.cmd()) + " " + result.errmsg());
        return false;
    }

    this->reportProgress(4, 100, "reload disk", "");

    QString installDir = partition;
#ifdef Q_OS_UNIX
    installDir = XSys::DiskUtil::MountPoint(partition);
    if (installDir.isEmpty()) {
        qCritical() << "Error::get(Error::USBMountFailed)";
        emit finished(Error::USBMountFailed, Error::get(Error::USBMountFailed));
        return false;
    }
#endif

    this->reportProgress(7, 100, "ClearTargetDev", "");
    qDebug() << "ClearTargetDev";
    Utils::ClearTargetDev(installDir);

    this->reportProgress(9, 100, "extract files", "");
    qDebug() << "end";
    SevenZip sevenZip(image, installDir);
    connect(sevenZip.m_szpp, &SevenZipProcessParser::progressChanged, m_bmr, &BMReporter::reportSevenZipProgress);

    if (!sevenZip.extract()) {
        qCritical() << "Error::get(Error::ExtractImgeFailed)";
        emit finished(Error::ExtractImgeFailed, Error::get(Error::ExtractImgeFailed));
        return false;
    }

    this->reportProgress(95, 100, "extract files", "");
    XSys::Bootloader::Syslinux::ConfigSyslinx(installDir);

#ifdef Q_OS_UNIX
    qDebug() << ("Syncing filesystems");
    XSys::SynExec("sync", "");
#endif

    this->reportProgress(99, 100, "extract files", "");
    XSys::DiskUtil::EjectDisk(partition);
    return true;
}
