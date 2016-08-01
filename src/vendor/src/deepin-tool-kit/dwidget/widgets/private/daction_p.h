#ifndef DACTION_P_H
#define DACTION_P_H

#include "daction.h"

#include <private/dobject_p.h>

DWIDGET_BEGIN_NAMESPACE

class DActionPrivate : public DObjectPrivate
{
    DActionPrivate(DAction *qq);

    DMenu *menu = Q_NULLPTR;

    QVariantMap toVariantMap() const;

    friend class DMenuPrivate;

    D_DECLARE_PUBLIC(DAction)
};

DWIDGET_END_NAMESPACE

#endif // DACTION_P_H
