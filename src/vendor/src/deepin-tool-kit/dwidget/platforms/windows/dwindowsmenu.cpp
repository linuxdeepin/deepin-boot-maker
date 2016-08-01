#include "dwindowsmenu.h"
#include "private/dmenu_p.h"
#include "private/daction_p.h"

#include <QDebug>
#include <QVariant>
#include <QVariantMap>
#include <QJsonDocument>
#include <QEventLoop>

DWIDGET_BEGIN_NAMESPACE

DMenuPrivate::DMenuPrivate(DMenu *qq)
    : DObjectPrivate(qq)
{

}

QString DMenuPrivate::createActionId(DAction *action) const
{
    if(parent)
        return parent->d_func()->createActionId(action);

    if(actionToId.contains(action))
        return actionToId.value(action);

    QString id = QString::number(actionId++);

    actionToId[action] = id;

    return id;
}

QVariantMap DMenuPrivate::toVariantMap() const
{
    QVariantList items;
    QVariantMap menuJsonContent;
    QVariantMap menuMap = menuVariant;

    for (DAction *action : menuActions) {
        if (action->menu())
            action->menu()->d_func()->parent = const_cast<DMenu*>(q_func());

        QVariantMap map = action->d_func()->toVariantMap();

        map["itemId"] = createActionId(action);

        items.push_back(map);
    }

    menuJsonContent["items"] = items;

    QJsonDocument menuJsonContentDoc = QJsonDocument::fromVariant(menuJsonContent);
    auto menuJsonContentStr = menuJsonContentDoc.toJson();
    menuMap["menuJsonContent"] = menuJsonContentStr;

    return menuMap;
}

QVariantMap DMenuPrivate::toSubVariantMap() const
{
    QVariantList items;
    QVariantMap menuJsonContent;

    for (DAction *action : menuActions) {
        QVariantMap map = action->d_func()->toVariantMap();

        map["itemId"] = createActionId(action);

        items.push_back(map);
    }

    menuJsonContent["items"] = items;

    return menuJsonContent;
}

void DMenuPrivate::_q_onItemInvoked(const QString &actionId, bool checked)
{
    DAction *action = actionToId.key(actionId);

    lastTriggerAction = action;

    if(!action)
        return;

    action->setChecked(checked);
    action->trigger();

    D_Q(DMenu);

    Q_EMIT q->triggered(action);

    if(!menuActions.contains(action)) {
        DMenu *actionMenu = qobject_cast<DMenu*>(action->parent());

        if(actionMenu) {
            actionMenu->triggered(action);
        }
    }
}

void DMenuPrivate::_q_onMenuUnregistered()
{
//    if (menuInterface) {
//        menuInterface->deleteLater();
//        menuInterface = Q_NULLPTR;
//    }

    if(eventLoop) {
        eventLoop->quit();
    }
}

void DMenu::attatch(QWidget *w)
{
    //D_D(DMenu);
    this->setParent(w);
}

DMenu::DMenu(QObject *parent)
    : QObject(parent)
    , DObject(* new DMenuPrivate(this))
{
    D_D(DMenu);

//    d->menuManager = new MenumanagerInterface(MenuManager_service,
//            MenuManager_path,
//            QDBusConnection::sessionBus(),
//            this);

//    d->menuInterface = Q_NULLPTR;
    d->menuVariant["x"] = 0;
    d->menuVariant["y"] = 0;
    d->menuVariant["isDockMenu"] = false;
    d->menuVariant["menuJsonContent"] = "";
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
    D_D(DMenu);

    d->menuActions << action;
}

DAction *DMenu::addMenu(DMenu *menu)
{
    D_D(DMenu);

    DAction *action = new DAction(this);

    menu->d_func()->parent = this;
    action->setMenu(menu);
    d->menuActions << action;

    return action;
}

DMenu *DMenu::addMenu(const QString &title)
{
    DMenu *menu = new DMenu(this);

    DAction *action = addMenu(menu);

    action->setText(title);

    return menu;
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

DAction *DMenu::actionAt(const QString &text) const
{
    D_DC(DMenu);

    for (DAction *action : d->menuActions) {
        if (action->text() == text) {
            return action;
        }
    }

    return Q_NULLPTR;
}

DAction *DMenu::actionAt(int index) const
{
    D_DC(DMenu);

    return d->menuActions.value(index);
}

QList<DAction *> DMenu::actionList() const
{
    D_DC(DMenu);

    return d->menuActions;
}

DAction *DMenu::exec()
{
    return exec(QCursor::pos());
}

DAction *DMenu::exec(const QPoint &p, DAction *action)
{
    D_D(DMenu);

    if(!d->eventLoop && popup(p, action)) {
        d->eventLoop = new QEventLoop(this);
        d->eventLoop->exec(QEventLoop::DialogExec);
        d->eventLoop->deleteLater();
        d->eventLoop = Q_NULLPTR;

        return d->lastTriggerAction;
    }

    return Q_NULLPTR;
}

bool DMenu::popup(const QPoint &pos, DAction */*action*/)
{
    D_D(DMenu);

    if (d->parent) {
        qWarning() << "Only top DMenu can be show";
        return false;
    }

//    if (d->menuInterface) {
//        qWarning() << "Another menu is active";
//        return false;
//    }

    d->menuVariant["x"] = pos.x();
    d->menuVariant["y"] = pos.y();

    QVariantMap content = d->toVariantMap();
    QJsonDocument jsonDoc = QJsonDocument::fromVariant(content);
    auto menuStr = jsonDoc.toJson();

//    QDBusPendingReply<QDBusObjectPath> reply = d->menuManager->RegisterMenu();
//    reply.waitForFinished();
//    if (reply.isError()) {
//        qDebug() << "Call deepin-menu dbus failed: " << reply.error();
//    }
//    QString menuPath = reply.value().path();
//    d->menuInterface = new MenuInterface(MenuManager_service, menuPath, QDBusConnection::sessionBus(), this);
//    d->menuInterface->ShowMenu(menuStr);

//    qDebug()<<d;
//    connect(d->menuInterface, &MenuInterface::MenuUnregistered, this, [&] {
//        D_D(DMenu);
//        d->_q_onMenuUnregistered();
//    });

//    connect(d->menuInterface, &MenuInterface::ItemInvoked, this, [&] (const QString &itemId, bool checked) {
//        D_D(DMenu);
//        d->_q_onItemInvoked(itemId, checked);
//    });

    return true;
}

DWIDGET_END_NAMESPACE

