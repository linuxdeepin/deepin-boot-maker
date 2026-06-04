// SPDX-FileCopyrightText: 2022-2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "logmanagertest.h"

// Sets up the test fixture.
void LogManagerTest::SetUp()
{

}

// Tears down the test fixture.
void LogManagerTest::TearDown()
{

}

TEST_F(LogManagerTest, setSystemlog)
{
//    EXPECT_EQ(DBMLogManager::setSystemLog(),true);

    DBMLogManager::setSystemLog(true);
    // 恢复状态，避免影响后续 registerFileAppender 测试
    // systemLog=true 时日志路径切换到 /var/log/deepin/，普通用户无写权限
    DBMLogManager::setSystemLog(false);
}

TEST_F(LogManagerTest, registerConsoleAppender)
{
    DBMLogManager::registerConsoleAppender();
}


TEST_F(LogManagerTest, registerFileppender)
{
    DBMLogManager::registerFileAppender();
}

TEST_F(LogManagerTest, getlogFilePath)
{
    QString ret = DBMLogManager::getlogFilePath();
    qInfo() << "ret = " << ret;
}

TEST_F(LogManagerTest, setLogFormat)
{
    QString format = "%{time}{yyyy-MM-dd, HH:mm:ss.zzz} [%{type:-7}] [%{file:-20} %{function:-35} %{line}] %{message}\n";
    DBMLogManager::setLogFormat(format);
}
