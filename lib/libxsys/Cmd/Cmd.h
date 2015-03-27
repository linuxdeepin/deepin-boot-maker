#pragma once

class QString;

#include "../Common/Result.h"

namespace XSys {

Result SynExec(const QString &exec, const QString &param, const QString &execPipeIn="");

}
