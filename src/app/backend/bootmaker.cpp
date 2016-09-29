#include "bootmaker.h"

#include <QDebug>
#include <QtConcurrent>
#include <XSys>

#include "../util/sevenzip.h"
#include "../util/utils.h"
#include "../util/bootmakeragent.h"
#include "../util/usbdevicemonitor.h"

#include "diskutil.h"

class Error
{
public:
    enum ErrorType {
        NoError = 0,
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
    m_porgressReporter = new BootMakerBackendDaemon(s_localPathUI, s_localPathBk);
    m_porgressReporter->initSendConnect();

    QThread *reportWork = new QThread;
    m_porgressReporter->moveToThread(reportWork);
    connect(this, &BootMaker::reportProgress, m_porgressReporter, &BootMakerBackendDaemon::reportProgress);
    connect(m_porgressReporter, &BootMakerBackendDaemon::start, this, &BootMaker::install);

    reportWork->start();

    m_usbDeviceMonitor = new UsbDeviceMonitor;
    QThread *monitorWork = new QThread;
    m_usbDeviceMonitor->moveToThread(monitorWork);
    connect(m_usbDeviceMonitor, &UsbDeviceMonitor::removePartitionsChanged,
            m_porgressReporter, &BootMakerBackendDaemon::sendRemovePartitionsChangedNotify);
    connect(monitorWork, &QThread::started,
            m_usbDeviceMonitor, &UsbDeviceMonitor::startMonitor);
    monitorWork->start();

    connect(this, &BootMaker::finished, this, [ = ](int errcode, const QString &description) {
        this->reportProgress(100, errcode, "install failed", description);
    });
}

bool BootMaker::install(const QString &image, const QString &unused_device, const QString &partition, bool formatDevice)
{
    m_usbDeviceMonitor->pauseMonitor();

    qDebug() << image << unused_device << partition << formatDevice;

    QString device = XSys::DiskUtil::GetPartitionDisk(partition);
    this->reportProgress(1, Error::NoError, "install bootloader", "");

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

    this->reportProgress(4, Error::NoError, "reload disk", "");

    QString installDir = partition;
#ifdef Q_OS_UNIX
    installDir = XSys::DiskUtil::MountPoint(partition);
    if (installDir.isEmpty()) {
        qCritical() << "Error::get(Error::USBMountFailed)";
        emit finished(Error::USBMountFailed, Error::get(Error::USBMountFailed));
        return false;
    }
#endif

    this->reportProgress(7, Error::NoError, "ClearTargetDev", "");
    qDebug() << "ClearTargetDev";
    Utils::ClearTargetDev(installDir);

    this->reportProgress(9, Error::NoError, "extract files", "");
    qDebug() << "end";
    SevenZip sevenZip(image, installDir);
    connect(sevenZip.m_szpp, &SevenZipProcessParser::progressChanged, m_porgressReporter, &BootMakerBackendDaemon::reportSevenZipProgress);

    if (!sevenZip.extract()) {
        qCritical() << "Error::get(Error::ExtractImgeFailed)";
        emit finished(Error::ExtractImgeFailed, Error::get(Error::ExtractImgeFailed));
        return false;
    }

    this->reportProgress(95, Error::NoError, "extract files", "");
    XSys::Bootloader::Syslinux::ConfigSyslinx(installDir);

#ifdef Q_OS_UNIX
    qDebug() << ("Syncing filesystems");
    XSys::SynExec("sync", "");
#endif

    this->reportProgress(99, Error::NoError, "extract files", "");
    XSys::DiskUtil::EjectDisk(partition);

    this->reportProgress(100, Error::NoError, "finish", "");
    return true;
}
