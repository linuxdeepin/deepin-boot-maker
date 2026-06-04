// SPDX-FileCopyrightText: 2022-2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include <QCoreApplication>
#include <gtest/gtest.h>
#include <csignal>
#ifdef QT_DEBUG
#include <sanitizer/asan_interface.h>
#endif

// 覆盖率数据刷新：确保程序退出（含异常退出）时 .gcda 文件被正确写入
extern "C" void __gcov_dump();

static void flushCoverage()
{
    __gcov_dump();
}

// 信号处理：捕获段错误等致命信号，先刷新覆盖率数据再退出
static void crashHandler(int sig)
{
    __gcov_dump();
    _exit(128 + sig);
}

int main(int argc, char *argv[])
{
    // 注册信号处理器，确保崩溃时覆盖率数据不丢失
    signal(SIGSEGV, crashHandler);
    signal(SIGABRT, crashHandler);
    signal(SIGBUS, crashHandler);

    // 注册 atexit 回调，在程序正常退出时刷新覆盖率数据
    atexit(flushCoverage);

    QCoreApplication a(argc, argv);
    a.setOrganizationName("deepin");
    a.setApplicationName("deepin-boot-maker");

    ::testing::InitGoogleTest(&argc,argv);
    int ret = RUN_ALL_TESTS();

    // 主动刷新覆盖率数据
    __gcov_dump();

#ifdef QT_DEBUG
    __sanitizer_set_report_path("asan_loader.log");
#endif
    return ret;
}
