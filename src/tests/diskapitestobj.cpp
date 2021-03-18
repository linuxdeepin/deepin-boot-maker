#include "diskapitestobj.h"

#include <DiskUtil/DiskUtil.h>
#include <QDebug>

QString getDiskKey(QString targetDev)
{
    int iIndex = targetDev.lastIndexOf("/");

    if (iIndex < 0) {
        return targetDev;
    }

    QString strPath = targetDev.left(iIndex + 1);
    QString strKey = targetDev.right(targetDev.length() - iIndex - 1);
    return strKey;
}

// Sets up the test fixture.
void DiskApiTestObj::SetUp()
{
    m_pBootMaker = new BootMaker;
}

// Tears down the test fixture.
void DiskApiTestObj::TearDown()
{
    delete  m_pBootMaker;
    m_pBootMaker = nullptr;
}

TEST_F(DiskApiTestObj, TestName)
{
    qInfo() << "Mount Point Test";
    QList<DeviceInfo> listDeviceInfo = m_pBootMaker->deviceList();

    if (listDeviceInfo.size() > 0) {
        DeviceInfo devInfo = listDeviceInfo.at(0);
        QString strDisk = XSys::DiskUtil::GetPartitionDisk(devInfo.path);
        qInfo() << "Disk Name" << strDisk;
        ASSERT_TRUE(!strDisk.isEmpty());
        QString strDiskKey = getDiskKey(strDisk);
        QStringList listPartions = XSys::DiskUtil::GetPartionOfDisk(strDiskKey);

        foreach (QString strPation, listPartions) {
            qDebug() << "Partion:" << strPation;
        }

        QString strMountPt = XSys::DiskUtil::MountPoint(devInfo.path);
        qInfo() << "Mount Point:" << strMountPt;

        if (!strMountPt.isEmpty()) {
            bool bRet = XSys::DiskUtil::UmountPartion(devInfo.path);
            ASSERT_TRUE(bRet);
            strMountPt = XSys::DiskUtil::MountPoint(devInfo.path);
            qInfo() << "Mount Point:" << strMountPt;
            ASSERT_TRUE(strMountPt.isEmpty());
        }

        bool bRet = XSys::DiskUtil::Mount(devInfo.path);
        ASSERT_TRUE(bRet);
        strMountPt = XSys::DiskUtil::MountPoint(devInfo.path);
        qInfo() << "Mount Point:" << strMountPt;
        ASSERT_FALSE(strMountPt.isEmpty());

        listPartions = XSys::DiskUtil::GetPartionOfDisk(strDiskKey);
        ASSERT_GE(listPartions.size(), 1);

        bRet = XSys::DiskUtil::UmountDisk(strDisk);
        listPartions.clear();
        strMountPt = XSys::DiskUtil::MountPoint(devInfo.path);
        ASSERT_TRUE(!strMountPt.isNull());
    }
}

TEST_F(DiskApiTestObj, TestName2)
{
    qDebug() << "Format Partion Test";
    QList<DeviceInfo> listDeviceInfo = m_pBootMaker->deviceList();

    if (listDeviceInfo.size() > 0) {
        DeviceInfo devInfo = listDeviceInfo.at(0);
        QString strMountPt = XSys::DiskUtil::MountPoint(devInfo.path);
        bool bRet = false;

        if (!strMountPt.isEmpty()) {
            bRet = XSys::DiskUtil::UmountPartion(devInfo.path);
        }
        else {
            bRet = true;
        }

        if (bRet) {
            //此处理需要root权限来进行操作，因而也会失败，报告日志为权限不足。
            bRet = XSys::DiskUtil::FormatPartion(devInfo.path);
            ASSERT_TRUE(bRet);
        }
        else {
            bRet = false;
            ASSERT_FALSE(bRet);
        }

        if (bRet) {
            QString strDisk = XSys::DiskUtil::GetPartitionDisk(devInfo.path);
            ASSERT_TRUE(!strDisk.isEmpty());

            //此处理需要root权限来进行操作，因而也会失败，报告日志为权限不足。
            bRet = XSys::DiskUtil::SetActivePartion(strDisk, devInfo.path);
            ASSERT_FALSE(bRet);
        }
    }
}
