#!/bin/bash

# SPDX-FileCopyrightText: 2022-2026 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-only

BUILD_DIR=build
REPORT_DIR=report

# 自动检测 qmake6 / qmake
if command -v qmake6 &>/dev/null; then
    QMAKE=qmake6
else
    QMAKE=qmake
fi

echo "Using qmake: $QMAKE ($($QMAKE --version 2>&1 | tail -1))"

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

rm -rf $BUILD_DIR
mkdir $BUILD_DIR
cd $BUILD_DIR

# 覆盖率构建不用 CONFIG+=debug，避免 ASAN 导致退出时崩溃、.gcda 文件丢失
$QMAKE CONFIG+=release ../tests.pro

TESTARGS="--gtest_output=xml:deepin_test_report_boot_maker.xml" make check -j$(nproc)

# ASAN 日志处理
if ls asan_loader.log* 1>/dev/null 2>&1; then
    mv asan_loader.log* asan_dde-boot-maker.log
else
    touch asan_dde-boot-maker.log
fi

# 生成覆盖率报告
lcov -d ./ -c -o coverage_all.info
lcov --remove coverage_all.info "*/tests/*" "*/usr/include*" --output-file coverage.info
cd ..
genhtml -o $REPORT_DIR $BUILD_DIR/coverage.info
#rm -rf $BUILD_DIR

exit 0
