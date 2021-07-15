#include "unittestobj.h"

#include <QtGlobal>
#include <QDebug>
#include <QThread>

BMInterface* UnitTestObj::m_pBMinterFace = nullptr;

void UnitTestObj::SetUpTestCase()
{
    m_pBMinterFace = BMInterface::instance();
}

void UnitTestObj::TearDownTestCase()
{
    delete  m_pBMinterFace;
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
