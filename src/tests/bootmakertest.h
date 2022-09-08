// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#ifndef BOOTMAKERTEST_H
#define BOOTMAKERTEST_H
#include <gtest/gtest.h>
#include "../libdbm/backend/bootmaker.h"

class BootMakerTest : public testing::Test
{
protected:
    // Sets up the test fixture.
    virtual void SetUp();

    // Tears down the test fixture.
    virtual void TearDown();
public:
    BootMaker *m_bootMaker;
};

#endif // BOOTMAKERTEST_H
