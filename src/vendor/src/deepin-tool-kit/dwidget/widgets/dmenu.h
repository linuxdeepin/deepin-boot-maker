#pragma once

#include <qglobal.h>

#ifdef Q_OS_LINUX
#include "ddeepinmenu.h"
#else
#include "dwindowsmenu.h"
#endif
