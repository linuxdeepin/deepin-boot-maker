#include "imagecheckobj.h"

// Sets up the test fixture.
void ImageCheckObj::SetUp()
{
    m_pBootMaker = new BootMaker;
}

// Tears down the test fixture.
void ImageCheckObj::TearDown()
{
    delete  m_pBootMaker;
    m_pBootMaker = nullptr;
}

TEST_F(ImageCheckObj, TestName)
{
    //由于DBus的调用检测功能，该函数会失败，如果需要成功测试，需要先去掉检测功能 checkcaller
    bool bResult = m_pBootMaker->checkfile("/home/sfs/Documents/test.iso");
    ASSERT_EQ(bResult, true);
    //bResult = m_pBootMaker->checkfile("/home/sfs/Documents/bad.iso");
    //ASSERT_EQ(bResult, false);
}