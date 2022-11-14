// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#ifndef BOOTMAKERSERVICETEST_H
#define BOOTMAKERSERVICETEST_H
#include <gtest/gtest.h>
#include <../service/bootmakerservice.h>


class BootMakerServiceTest : public testing::Test
{
protected:
    // Sets up the test fixture.
    virtual void SetUp();

    // Tears down the test fixture.
    virtual void TearDown();
public:
    BootMakerService *m_bootMakerService;
};

#endif // BOOTMAKERSERVICETEST_H
