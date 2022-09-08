#!/bin/bash

# SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-only

BUILD_DIR=build
REPORT_DIR=report

cd ../../

rm -rf $BUILD_DIR
mkdir $BUILD_DIR

cd $BUILD_DIR

qmake  ../
make

cd ../src/tests/
rm -rf $BUILD_DIR
mkdir $BUILD_DIR
cd $BUILD_DIR

qmake CONFIG+=debug ../

TESTARGS="--gtest_output=xml:deepin_test_report_boot_maker.xml"  make check -j$(nproc)

mv asan_loader.log* asan_dde-boot-maker.log

[ -e asan_loader.log* ] && mv asan_loader.log* asan_dde-boot-maker.log || touch asan_dde-boot-maker.log

lcov -d ./ -c -o coverage_all.info

lcov --remove coverage_all.info "*/tests/*" "*/usr/include*" --output-file coverage.info
cd ..
genhtml -o $REPORT_DIR $BUILD_DIR/coverage.info
#rm -rf $BUILD_DIR

exit 0