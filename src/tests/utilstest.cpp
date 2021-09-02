#include "utilstest.h"

void UtilsTest::SetUp()
{
//    qInfo() << "Test SetUp";
}

void UtilsTest::TearDown()
{
//    qInfo() << "Test TearDown";
}

TEST_F(UtilsTest, utils)
{
    Utils::initResource();
    Utils::loadTranslate();
    Utils::ClearTargetDev("xxxkjh");
    Utils::CheckInstallDisk("xxxkjh");
    Utils::CommandDfParse();
    Utils::CommandLsblkParse();
    Utils::ListUsbDrives();
    Utils::UsbShowText("xxxkjh");
}
