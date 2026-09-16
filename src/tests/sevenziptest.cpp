// SPDX-FileCopyrightText: 2022 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

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

// New 7-Zip exposes the El Torito boot image as a synthetic entry with the
// real size (here 5.26 GiB); it must be detected as oversize.
TEST_F(SevenZipTest, ParseOversizeBootNewFormat)
{
    const QString slt = QString::fromLatin1(R"(Path = [BOOT]
Folder = +
Size = 
Packed Size = 
Modified = 2026-09-14 09:52:37
Mode = 
Symbolic Link = 
Path = [BOOT]/Boot-NoEmul.img
Folder = -
Size = 0
Packed Size = 0
Modified = 2026-09-14 09:42:40
Mode = 
Symbolic Link = 
Path = [BOOT]/Boot-NoEmul.img
Folder = -
Size = 5647439872
Packed Size = 5647439872
Modified = 
Mode = 
Symbolic Link = 
)");
    const QStringList paths = SevenZip::parseOversizeBootPaths(slt);
    ASSERT_EQ(paths.size(), 1);
    EXPECT_EQ(paths.first(), QStringLiteral("[BOOT]/Boot-NoEmul.img"));
}

// Old p7zip does not expose the synthetic [BOOT] entry, so nothing is
// excluded and the previous behavior is preserved.
TEST_F(SevenZipTest, ParseOversizeBootOldFormat)
{
    const QString slt = QString::fromLatin1(R"(Path = live/filesystem.squashfs
Folder = -
Size = 4210458624
Packed Size = 4210458624
Modified = 2025-04-15 12:00:00
Attributes = A
CRC = 12345678
Encrypted = -
)");
    EXPECT_TRUE(SevenZip::parseOversizeBootPaths(slt).isEmpty());
}

// A small [BOOT] entry (7 MiB) is below the FAT32 limit and must be kept.
TEST_F(SevenZipTest, ParseOversizeBootSmallEntry)
{
    const QString slt = QString::fromLatin1(R"(Path = [BOOT]/Boot-NoEmul.img
Folder = -
Size = 7372800
Packed Size = 7372800
Modified = 
Mode = 
Symbolic Link = 
)");
    EXPECT_TRUE(SevenZip::parseOversizeBootPaths(slt).isEmpty());
}
