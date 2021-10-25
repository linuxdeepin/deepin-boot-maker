#include "sevenziptest.h"

void SevenZipTest::SetUp()
{
    m_sevenZip = new SevenZip("img","target");
}

void SevenZipTest::TearDown()
{
    delete  m_sevenZip;
    m_sevenZip = nullptr;
}

TEST_F(SevenZipTest, Public)
{
    m_sevenZip->setArchiveFile("xxdasf");
    m_sevenZip->setOutputDirectory("xxdasf");
    //m_sevenZip->stopProcess();
    //m_sevenZip->extract();
    m_sevenZip->check();
}
