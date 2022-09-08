// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#ifndef IMAGECHECKOBJ_H
#define IMAGECHECKOBJ_H

#include <gtest/gtest.h>
#include "backend/bootmaker.h"
#include "util/utils.h"

class ImageCheckObj : public ::testing::Test
{
protected:
    // Sets up the test fixture.
    virtual void SetUp();

    // Tears down the test fixture.
    virtual void TearDown();

public:
    BootMaker* m_pBootMaker;
};

#endif // IMAGECHECKOBJ_H
