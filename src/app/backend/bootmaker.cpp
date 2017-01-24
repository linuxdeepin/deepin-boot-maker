#include "bootmaker.h"

#include <QDebug>
#include <XSys>

#include "../util/sevenzip.h"
#include "../util/utils.h"
#include "../util/devicemonitor.h"

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
    case NoError:
        return "";
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
    }
    return QObject::tr("Internal Error");
}

BootMaker::BootMaker(QObject *parent) : BMHandler(parent)
{
    qRegisterMetaType<QList<DeviceInfo> >();

    m_usbDeviceMonitor = new DeviceMonitor;
    QThread *monitorWork = new QThread;
    m_usbDeviceMonitor->moveToThread(monitorWork);
    connect(monitorWork, &QThread::started,
            m_usbDeviceMonitor, &DeviceMonitor::startMonitor);
    connect(m_usbDeviceMonitor, &DeviceMonitor::removablePartitionsChanged,
            this, &BootMaker::removablePartitionsChanged);

    monitorWork->start();

    connect(this, &BootMaker::finished, this, [ = ](int errcode, const QString & description) {
        this->reportProgress(100, errcode, "install failed", description);
    });
}

const QList<DeviceInfo> BootMaker::deviceList() const
{
    return m_usbDeviceMonitor->deviceList();
}

bool BootMaker::install(const QString &image, const QString &unused_device, const QString &partition, bool formatDevice)
{
    emit m_usbDeviceMonitor->pauseMonitor();

    qDebug() << image << unused_device << partition << formatDevice;

    QString device = XSys::DiskUtil::GetPartitionDisk(partition);

    this->reportProgress(5, Error::NoError, "install bootloader", "");
    qDebug() << "begin install bootloader";
    QString targetPartition = partition;
    XSys::Result result;
    if (formatDevice) {
        result = XSys::Bootloader::InstallBootloader(device);
        targetPartition = result.result();
    } else {
        result = XSys::Bootloader::Syslinux::InstallSyslinux(partition);
    }
    qDebug() << "install bootloader finish: " << result.isSuccess();

    if (! result.isSuccess()) {
        qCritical() << "install bootloader failed: "<< result.errmsg();
        emit finished(Error::SyscExecFailed, Error::get(Error::SyscExecFailed).arg(result.cmd()) + " " + result.errmsg());
        return false;
    }

    this->reportProgress(10, Error::NoError, "begin reload disk", "");
    qDebug() << "begin reload disk";

    QString installDir = partition;
#ifdef Q_OS_UNIX
    installDir = XSys::DiskUtil::MountPoint(partition);
    if (installDir.isEmpty()) {
        qCritical() << "Error::get(Error::USBMountFailed)";
        emit finished(Error::USBMountFailed, Error::get(Error::USBMountFailed));
        return false;
    }
#endif

    this->reportProgress(15, Error::NoError, "clear target device files", "");
    qDebug() << "begin clear target device files";
    Utils::ClearTargetDev(installDir);

    this->reportProgress(20, Error::NoError, "extract files", "");
    qDebug() << "begin extract files";
    SevenZip sevenZip(image, installDir);
    connect(sevenZip.m_szpp, &SevenZipProcessParser::progressChanged,
    this, [ = ](int current, int total, const QString & fileName) {
        qDebug() << current << total << fileName;
        this->reportProgress(current, Error::NoError, "extract files", "");
    });

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
