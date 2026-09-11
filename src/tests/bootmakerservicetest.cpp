// SPDX-FileCopyrightText: 2022 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "bootmakerservicetest.h"
#include "../service/bootmakerservice_p.h"
#include <QTemporaryFile>
#include <QDBusUnixFileDescriptor>
#include <unistd.h>
#include <QDebug>

// Sets up the test fixture.
void BootMakerServiceTest::SetUp()
{
    m_bootMakerService = new BootMakerService;
    m_bootMakerService->d_ptr->disableCheck = true;
}

// Tears down the test fixture.
void BootMakerServiceTest::TearDown()
{
    delete m_bootMakerService;
    m_bootMakerService = nullptr;
}

TEST_F(BootMakerServiceTest, Signal)
{
//    QString add = "add";
//    QString del = "del";

//    m_bootMakerService->DeviceListChanged(add,del);

//    int errcode = 0;
//    QString description;
//    m_bootMakerService->Finished(errcode,description);


//    int current = 0;
//    int error = -1;
//    QString title = "";
//    QString descript = "";
//    m_bootMakerService->ReportProgress(current, error, title, descript);

//    m_bootMakerService->ReportProgress1(current, error, title, descript);
}

TEST_F(BootMakerServiceTest, Slots)
{
    m_bootMakerService->Reboot();
    m_bootMakerService->Start();
    m_bootMakerService->Stop();

    QString ret = m_bootMakerService->DeviceList();
    qInfo() << "ret = " << ret;

    // Install/CheckFile only take a file descriptor now; open one so the fd is
    // valid (an invalid fd is rejected).
    QTemporaryFile isoFile;
    ASSERT_TRUE(isoFile.open());
    QDBusUnixFileDescriptor fd(dup(isoFile.handle()));

    EXPECT_TRUE(m_bootMakerService->Install("/dev/sdb", "/dev/sdb1", false, fd));
    EXPECT_TRUE(m_bootMakerService->CheckFile(fd));

    // No descriptor -> rejected.
    EXPECT_FALSE(m_bootMakerService->Install("/dev/sdb", "/dev/sdb1", false, QDBusUnixFileDescriptor()));
    EXPECT_FALSE(m_bootMakerService->CheckFile(QDBusUnixFileDescriptor()));
}
