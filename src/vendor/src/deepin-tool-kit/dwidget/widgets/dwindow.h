#ifndef DWINDOW_H
#define DWINDOW_H

#include <QWidget>
#include <dobject.h>
#include <dwidget_global.h>

#include "dwidget.h"
#include <DMenu>

DWIDGET_BEGIN_NAMESPACE

class DMenu;
class DWindowPrivate;

class LIBDTKWIDGETSHARED_EXPORT DWindow : public DWidget
{
    Q_OBJECT
public:
    explicit DWindow(QWidget *parent = 0);

    DMenu *dbusMenu();

    void setContentLayout(QLayout *l);
    void setContentWidget(QWidget *w);

    void setParent(QWidget *parent);
    QWidget *parentWidget() const;

protected slots:

private:
    D_DECLARE_PRIVATE(DWindow)
    Q_DISABLE_COPY(DWindow)
};

DWIDGET_END_NAMESPACE

#endif // DWINDOW_H
