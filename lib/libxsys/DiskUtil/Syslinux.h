#pragma once

#include <QString>
#include "../Common/Result.h"

namespace  XSys {
    namespace  Syslinux {
        Result InstallModule(const QString& installDirectory);
        Result InstallBootloader(const QString& device);
        Result InstallMbr(const QString& device);
    }
}
