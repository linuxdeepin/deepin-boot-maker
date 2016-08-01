#include "dwindow.h"

#include <QHBoxLayout>
#include <DMenu>
#include <DTitlebar>
#include <QDebug>
#include <private/dwidget_p.h>

DWIDGET_BEGIN_NAMESPACE

class DWindowPrivate: public DWidgetPrivate
{
    D_DECLARE_PUBLIC(DWindow)

public:
    explicit DWindowPrivate(DWindow *q);
    DMenu   *dbusMenu   = nullptr;
    QWidget *m_parent   = nullptr;
};

DWindowPrivate::DWindowPrivate(DWindow *q): DWidgetPrivate(q)
{

}

DWindow::DWindow(QWidget *parent): DWidget(*(new DWindowPrivate(this)))
{
    D_D(DWindow);

    setParent(parent);
    d->dbusMenu = new DMenu;
    d->dbusMenu->attatch(this);
    setTitlebarMenu(d->dbusMenu);

}

void DWindow::setParent(QWidget *parent)
{
    D_D(DWindow);
    d->m_parent = parent;
    QObject::setParent(parent);
}

QWidget *DWindow::parentWidget() const
{
    D_DC(DWindow);
    return d->m_parent;
}

DMenu *DWindow::dbusMenu()
{
    D_D(DWindow);
    return d->dbusMenu;
}

void DWindow::setContentLayout(QLayout *l)
{
    DWidget::setLayout(l);
}

void DWindow::setContentWidget(QWidget *w)
{
    QHBoxLayout *l = new QHBoxLayout;
    l->setSpacing(0);
    l->setMargin(0);
    l->addWidget(w);
    l->setAlignment(w, Qt::AlignCenter);
    setContentLayout(l);
}

DWIDGET_END_NAMESPACE
