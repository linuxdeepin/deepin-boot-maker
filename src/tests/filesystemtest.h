// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#ifndef FILESYSTEMTEST_H
#define FILESYSTEMTEST_H

#include <gtest/gtest.h>
#include "../libxsys/FileSystem/FileSystem.h"

class FileSystemTest : public testing::Test
{
protected:
    // Sets up the test fixture.
    virtual void SetUp();

    // Tears down the test fixture.
    virtual void TearDown();
public:
    QString m_fileA;
    QString m_fileB;
    QString m_dir;
};

#endif // FILESYSTEMTEST_H
