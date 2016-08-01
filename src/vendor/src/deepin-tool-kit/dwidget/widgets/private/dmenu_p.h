#ifndef DMENU_P_H
#define DMENU_P_H

#include "doption_p.h"
#include "dmenu.h"

class MenumanagerInterface;
class MenuInterface;

QT_BEGIN_NAMESPACE
class QEventLoop;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class DMenuPrivate: public DObjectPrivate
{
protected:
    DMenuPrivate(DMenu *qq);

private:
    DMenu *parent = Q_NULLPTR;
#ifdef Q_OS_LINUX
    MenumanagerInterface    *menuManager;
    MenuInterface           *menuInterface;
#endif
    QVariantMap             menuVariant;
    QList<DAction *>    menuActions;

    mutable int actionId = 0;
    mutable QMap<DAction*, QString> actionToId;
    QEventLoop *eventLoop = Q_NULLPTR;
    DAction *lastTriggerAction = Q_NULLPTR;

    QString createActionId(DAction *action) const;
    QVariantMap toVariantMap() const;
    QVariantMap toSubVariantMap() const;

    void _q_onMenuUnregistered();
    void _q_onItemInvoked(const QString &actionId, bool checked);

    friend class DActionPrivate;

    D_DECLARE_PUBLIC(DMenu)
};

DWIDGET_END_NAMESPACE

#endif // DMENU_P_H
