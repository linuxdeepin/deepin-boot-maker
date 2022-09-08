// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "bootmakerservicetest.h"
#include <QDebug>
// Sets up the test fixture.
void BootMakerServiceTest::SetUp()
{
    m_bootMakerService = new BootMakerService;
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

    EXPECT_TRUE(m_bootMakerService->Install("~/Downloads/uniontechos-desktop-21.0-home-beta6-amd64.iso","/dev/sdb","/dev/sdb1",false));

    EXPECT_TRUE(m_bootMakerService->CheckFile("/dev/sdb1"));
}
