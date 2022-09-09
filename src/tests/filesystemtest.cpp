// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "filesystemtest.h"
#include <QFile>
#include <QDir>
#include <QDebug>

// Sets up the test fixture.
void FileSystemTest::SetUp()
{
    m_fileA = "/tmp/lls.txt";
    m_fileB = "/tmp/llls.txt";
    m_dir = "/tmp/testDir";
    QStringList list;
    list << m_fileA << m_fileB;
    for (int i = 0; i < list.size(); ++i) {
        QFile f(list[i]);
        if (!f.open(QIODevice::ReadWrite)) {
            return;
        }
        f.write("test");
        f.close();
    }

    QDir dir;
    dir.mkdir(m_dir);

}

// Tears down the test fixture.
void FileSystemTest::TearDown()
{
    QFile fileA(m_fileA);
    QFile fileB(m_fileB);
    if (fileA.exists() || fileB.exists()) {
        QFile::remove(m_fileA);
        QFile::remove(m_fileB);
    }
    QDir dir(m_dir);
    if (dir.exists()) {
        dir.rmdir(m_dir);
    }
}

TEST_F(FileSystemTest,fileTest)
{
    QFile fileA(m_fileA);
    QFile fileB(m_fileB);


    if (fileA.exists() || fileB.exists()) {
         QByteArray bateArray = "hhhhh";
         EXPECT_TRUE(XSys::FS::InsertFileData(m_fileA,bateArray));
         EXPECT_TRUE(XSys::FS::InsertFile(m_fileA,m_fileB));

         EXPECT_TRUE(XSys::FS::CpFile(m_fileA,m_fileB));

         QStringList fileList;
         fileList << "/tmp/a.txt" << "/tmp/b.txt";
         QString ret =  XSys::FS::PathSearch(m_fileA,fileList);
         qInfo() << "ret = " << ret;
         ret = XSys::FS::SearchBin(m_fileB);

         EXPECT_TRUE(XSys::FS::RmFile(m_fileA));
         EXPECT_TRUE(XSys::FS::RmFile(fileB));

         QString newDir = "/tmp/dirTest";
         EXPECT_TRUE(XSys::FS::MoveDir(m_dir,newDir));

         EXPECT_TRUE(XSys::FS::RmDir(newDir));


    }


}

//TEST_F(FileSystemTest,RmFile)
//{
//    EXPECT_TRUE(XSys::FS::RmFile("~/ls.txt"));
//}

//TEST_F(FileSystemTest,RmFileQFile)
//{
//    QFile f("~/hello.txt");
//    EXPECT_TRUE(XSys::FS::RmFile(f));
//}

//// 文件复制 需要两个文件都存在 才能复制
//TEST_F(FileSystemTest,CpFile)
//{
//    QString fileA = "~/hello.txt";
//    QString fileB = "~/test/fileB/hi.txt";
//    EXPECT_TRUE(XSys::FS::CpFile(fileA,fileB));
//}

//TEST_F(FileSystemTest,RmDir)
//{
//    QString dirPath = "~/test/testDir";
//    EXPECT_TRUE(XSys::FS::RmDir(dirPath));
//}


//TEST_F(FileSystemTest,MoveDir)
//{
//    QString oldName = "~/test/oldname";
//    QString newName = "~/test/newname/dir";
//    EXPECT_TRUE(XSys::FS::MoveDir(oldName,newName));
//}

//TEST_F(FileSystemTest,PathSearch)
//{
//#define TESTDIR "~/test/"
//#define DIR "~/"
//    QString file = TESTDIR"a.txt";
//    QStringList fileList;
//    fileList << DIR << TESTDIR;
//    QString ret =  XSys::FS::PathSearch(file,fileList);
//    qInfo() << "ret = " << ret;
//}

//TEST_F(FileSystemTest,SearchBin)
//{
//    QString bin = TESTDIR"z.txt";
//    QString ret =  XSys::FS::SearchBin(bin);
//    qInfo() << "ret = " << ret;
//}

//TEST_F(FileSystemTest,InsertFileData)
//{
//    QString file = TESTDIR"a.txt";
//    QByteArray bateArray = "hhhhh";
//    EXPECT_TRUE(XSys::FS::InsertFileData(file,bateArray));
//}

//TEST_F(FileSystemTest,InsertFile)
//{
//    QString fileA = TESTDIR"a.txt";
//    QString fileB = TESTDIR"c.txt";
//    EXPECT_TRUE(XSys::FS::InsertFile(fileA,fileB));
//}

//TEST_F(FileSystemTest,TmpFilePath)
//{
//    QString file = TESTDIR"a.txt";
//    QString ret = XSys::FS::TmpFilePath(file);
//    qInfo() << "ret = " << ret;
//}

//TEST_F(FileSystemTest,InsertTmpFile)
//{
//    QString file = TESTDIR"a.txt";
//    QString ret = XSys::FS::InsertTmpFile(file);
//    qInfo() << "ret = " << ret;
//}

