// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#ifndef UNITTESTOBJ_H
#define UNITTESTOBJ_H

#include <bminterface.h>
#include <gtest/gtest.h>

class UnitTestObj : public testing::Test
{
public:
    static void SetUpTestCase();
    static void TearDownTestCase();

protected:
    // Sets up the test fixture.
    virtual void SetUp() override;

    // Tears down the test fixture.
    virtual void TearDown() override;

public:
    static BMInterface* m_pBMinterFace;
};

#endif // UNITTESTOBJ_H
