// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#ifndef UTILSTEST_H
#define UTILSTEST_H
#include <gtest/gtest.h>
#include <../libdbm/util/utils.h>

class UtilsTest: public testing::Test
{
protected:
    // Sets up the test fixture.
    virtual void SetUp() override;

    // Tears down the test fixture.
    virtual void TearDown() override;

public:

};

#endif // UTILSTEST_H
