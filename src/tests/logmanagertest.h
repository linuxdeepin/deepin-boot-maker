// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#ifndef LOGMANAGERTEST_H
#define LOGMANAGERTEST_H
#include <gtest/gtest.h>
#include "../service/LogManager.h"

class LogManagerTest : public testing::Test
{
protected:
    // Sets up the test fixture.
    virtual void SetUp();

    // Tears down the test fixture.
    virtual void TearDown();
};

#endif // LOGMANAGERTEST_H
