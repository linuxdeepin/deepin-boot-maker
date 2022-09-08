// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once
#include "../Common/Result.h"

#include <QString>

namespace  XSys {
namespace  Syslinux {
Result InstallModule(const QString &installDirectory);
Result InstallBootloader(const QString &device);
Result InstallMbr(const QString &device);
Result ConfigSyslinx(const QString &targetDev);
}
}
