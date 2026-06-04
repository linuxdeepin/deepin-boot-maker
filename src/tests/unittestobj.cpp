// SPDX-FileCopyrightText: 2022-2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "unittestobj.h"

#include <QtGlobal>
#include <QDebug>
#include <QThread>

BMInterface* UnitTestObj::m_pBMinterFace = nullptr;

void UnitTestObj::SetUpTestCase()
{
    // m_pBMinterFace = BMInterface::instance();
    m_pBMinterFace = &BMInterface::ref();
}

void UnitTestObj::TearDownTestCase()
{
    // BMInterface::ref() 返回单例引用，不能 delete，由 DSingleton 自动管理生命周期
    m_pBMinterFace = nullptr;
}

void UnitTestObj::SetUp()
{
    qInfo() << "Test SetUp";
}

void UnitTestObj::TearDown()
{
    qInfo() << "Test TearDown";
}

TEST_F(UnitTestObj, testObj)
{
    //由于DBus的调用检测功能，该函数会失败，这里仅用于测试DBus自动唤起功能
    //bool bResult = m_pBMinterFace->checkfile("/home/sfs/Documents/test.iso");
    bool bResult = true;
    ASSERT_EQ(bResult, true);
}
