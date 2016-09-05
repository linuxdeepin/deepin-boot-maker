#ifndef DMAINWINDOW_P_H
#define DMAINWINDOW_P_H

#include "private/dobject_p.h"
#include "dwidget_global.h"
#include "dmainwindow.h"

DWIDGET_BEGIN_NAMESPACE

class DPlatformWindowHandle;
class DTitlebar;

class DMainWindowPrivate : public DObjectPrivate
{
public:
    DMainWindowPrivate(DMainWindow *qq);

    void init();

    DPlatformWindowHandle *handle = Q_NULLPTR;
    DTitlebar *titlebar = Q_NULLPTR;

private:
    D_DECLARE_PUBLIC(DMainWindow)
};

DWIDGET_END_NAMESPACE

#endif // DMAINWINDOW_P_H
