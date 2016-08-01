#include "daction.h"
#include "dmenu.h"
#include "private/daction_p.h"
#include "private/dmenu_p.h"

DWIDGET_BEGIN_NAMESPACE

DActionPrivate::DActionPrivate(DAction *qq)
    : DObjectPrivate(qq)
{

}

QVariantMap DActionPrivate::toVariantMap() const
{
    D_QC(DAction);

    QVariantMap menuItem;

    menuItem["itemText"]            = q->text();
    menuItem["itemExtra"]           = "";
    menuItem["isActive"]            = q->isEnabled();
    menuItem["checked"]             = q->isChecked();
    menuItem["isCheckable"]         = q->isCheckable();

#ifdef Q_OS_LINUX
    if (menu) {
        menuItem["itemSubMenu"]     = menu->d_func()->toSubVariantMap();
    }
#endif
    return menuItem;
}

DAction::DAction(QObject *parent)
    : QAction(parent)
    , DObject(*new DActionPrivate(this))
{

}

DAction::DAction(const QString &text, QObject *parent)
    : QAction(text, parent)
    , DObject(*new DActionPrivate(this))
{

}

DAction::DAction(const QIcon &icon, const QString &text, QObject *parent)
    : QAction(icon, text, parent)
    , DObject(*new DActionPrivate(this))
{

}

DAction::~DAction()
{
    D_D(DAction);

    if(d->menu)
        d->menu->deleteLater();
}

DMenu *DAction::menu() const
{
    D_DC(DAction);

    return d->menu;
}

void DAction::setMenu(DMenu *menu)
{
    D_D(DAction);

    d->menu = menu;
}

DWIDGET_END_NAMESPACE
