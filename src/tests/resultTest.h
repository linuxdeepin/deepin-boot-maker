// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#ifndef RESULT_TEST_H
#define RESULT_TEST_H

#include <gtest/gtest.h>

class ResultTest : public testing::Test
{
protected:
    // Sets up the test fixture.
    virtual void SetUp() override;

    // Tears down the test fixture.
    virtual void TearDown() override;
};

#endif
