#ifndef DSCROLLBAR_P_H
#define DSCROLLBAR_P_H

#include "dscrollbar.h"

#include <private/dobject_p.h>

QT_BEGIN_NAMESPACE
class QGraphicsOpacityEffect;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class DScrollBarPrivate : public DObjectPrivate
{
    DScrollBarPrivate(DScrollBar *qq);

    void init();
    void setOpacity(qreal opacity);
    void _q_hidden();
    void _q_updateOpacity();

    bool canPaint = true;
    bool autoHide = true;
    qreal opacity = 1;

    QTimer *timer = Q_NULLPTR;
    QTimer *opacityTimer = Q_NULLPTR;
    QGraphicsOpacityEffect *effect = Q_NULLPTR;

    D_DECLARE_PUBLIC(DScrollBar)
};

DWIDGET_END_NAMESPACE

#endif // DSCROLLBAR_P_H
