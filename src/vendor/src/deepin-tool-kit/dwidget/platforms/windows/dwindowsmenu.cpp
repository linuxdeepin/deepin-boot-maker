#include "dwindowsmenu.h"
#include "popupmenustyle.h"
#include "private/dmenu_p.h"
#include "daction.h"

#include <QDebug>

DWIDGET_BEGIN_NAMESPACE

DMenuPrivate::DMenuPrivate(DMenu *qq): DObjectPrivate(qq)
{

}

DMenu::DMenu(QWidget *parent): QMenu(parent), DObject(*new DMenuPrivate(this))
{
    setAttribute(Qt::WA_TranslucentBackground);
    setContentsMargins(1, 1, 1, 1);
    setStyle(new PopupMenuStyle());

    connect(this, &QMenu::triggered,
            [&](QAction *action){
        emit triggered(qobject_cast<DAction*>(action));
    });
}

DAction *DMenu::addAction(const QString &text)
{
    DAction *action = new DAction(text, this);
    addAction(action);
    return action;
}

DAction *DMenu::addAction(const QIcon &icon, const QString &text)
{
    DAction *action = new DAction(icon, text, this);
    addAction(action);
    return action;
}

void DMenu::addAction(DAction *action)
{
    QMenu::addAction(action);
}

DAction *DMenu::addMenu(DMenu *menu)
{
    DAction *action = new DAction(this);
    action->setMenu(menu);
    addAction(action);
    return action;
}

DMenu *DMenu::addMenu(const QString &title)
{
    DMenu *menu = new DMenu(this);
    DAction *action = addMenu(menu);
    action->setText(title);
    return action->menu();
}

DMenu *DMenu::addMenu(const QIcon &icon, const QString &title)
{
    DMenu *menu = new DMenu(this);
    DAction *action = addMenu(menu);
    action->setIcon(icon);
    action->setText(title);
    return menu;
}

DAction *DMenu::addSeparator()
{
    return addAction("");
}

//DAction *DMenu::actionAt(const QString &text) const
//{
//    D_DC(DMenu);

//    for (DAction *action : d->menuActions) {
//        if (action->text() == text) {
//            return action;
//        }
//    }

//    return Q_NULLPTR;
//}

//DAction *DMenu::actionAt(int index) const
//{
//    D_DC(DMenu);

//    return d->menuActions.value(index);
//}

//QList<DAction *> DMenu::actionList() const
//{
//    D_DC(DMenu);

//    return d->menuActions;
//}

DWIDGET_END_NAMESPACE
