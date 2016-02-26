/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#pragma once

class QString;

#include "../Common/Result.h"

namespace XSys {

Result SynExec(const QString &exec, const QString &param, const QString &execPipeIn="");

}
