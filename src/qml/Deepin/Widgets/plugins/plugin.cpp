#include "plugins/plugin.h"
#include "plugins/dwindow.h"
#include "plugins/dicon.h"

#include <qqml.h>

void DockAppletPlugin::registerTypes(const char *uri)
{
    //@uri Deepin.Widgets
    qmlRegisterType<DOverrideWindow>(uri, 1, 0, "DOverrideWindow");
    qmlRegisterType<DWindow>(uri, 1, 0, "DWindow");
    qmlRegisterType<DIcon>(uri, 1, 0, "DIcon");
}


