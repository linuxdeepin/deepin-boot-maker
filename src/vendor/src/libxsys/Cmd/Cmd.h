// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once
#include "../Common/Result.h"

class QString;

namespace XSys {

Result SynExec(const QString &exec, const QString &param, const QString &execPipeIn = "");
Result SynExec(const QString &exec, const QStringList &args);

}
