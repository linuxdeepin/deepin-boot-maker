#include "bootmakertest.h"

// Sets up the test fixture.
void BootMakerTest::SetUp()
{
    m_bootMaker = new BootMaker;
}

// Tears down the test fixture.
void BootMakerTest::TearDown()
{
    delete m_bootMaker;
    m_bootMaker = nullptr;
}

TEST_F(BootMakerTest, Slots)
{
    m_bootMaker->reboot();
    m_bootMaker->start();
    m_bootMaker->stop();
    m_bootMaker->deviceList();
    m_bootMaker->install("~/virtual_disk.img","/dev/sdb","/dev/sdb1",false);
    m_bootMaker->checkfile("/dev/sdb1");
}
