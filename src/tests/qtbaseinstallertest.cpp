// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "qtbaseinstallertest.h"

void QtBaseInstallerTest::SetUp()
{
    m_qtBaseInstaller = new QtBaseInstaller;
}

// Tears down the test fixture.
void QtBaseInstallerTest::TearDown()
{
    delete  m_qtBaseInstaller;
    m_qtBaseInstaller = nullptr;
}


TEST_F(QtBaseInstallerTest, Public)
{
    m_qtBaseInstaller->setformat(false);
    m_qtBaseInstaller->setPartionName("/dev/sdb1");
    m_qtBaseInstaller->setImage("~/Downloads/uniontechos-desktop-21.0-home-beta6-amd64.iso");
    EXPECT_FALSE(m_qtBaseInstaller->isRunning());

    m_qtBaseInstaller->beginInstall();
    m_qtBaseInstaller->checkError();
    m_qtBaseInstaller->stopInstall();
    m_qtBaseInstaller->checkISOIntegrity();
}

TEST_F(QtBaseInstallerTest, Protected)
{
//    EXPECT_TRUE(m_qtBaseInstaller->umountPartion());
}
