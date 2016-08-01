/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dscrollbar.h"
#include "dthememanager.h"

#include "private/dscrollbar_p.h"

#include <QPalette>
#include <QTimer>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>

DWIDGET_BEGIN_NAMESPACE

DScrollBarPrivate::DScrollBarPrivate(DScrollBar *qq)
    : DObjectPrivate(qq)
{

}

void DScrollBarPrivate::init()
{
    D_Q(DScrollBar);

    timer = new QTimer(q);
    opacityTimer = new QTimer(q);

    timer->setInterval(1000);
    timer->setSingleShot(true);

    q->connect(timer, SIGNAL(timeout()), q, SLOT(_q_hidden()));
    q->connect(opacityTimer, SIGNAL(timeout()), q, SLOT(_q_updateOpacity()));
    q->connect(q, &DScrollBar::valueChanged, q, [this] {
        timer->start();
        opacityTimer->stop();

        setOpacity(1);
    });
}

void DScrollBarPrivate::setOpacity(qreal opacity)
{
    D_Q(DScrollBar);

    if (qAbs(this->opacity - opacity) < 0.001) {
        return;
    }

    this->opacity = opacity;
    this->canPaint = false;

    q->update();
}

void DScrollBarPrivate::_q_hidden()
{
    timer->stop();
    opacityTimer->start(50);
}

void DScrollBarPrivate::_q_updateOpacity()
{
    if (opacity < 0.01) {
        opacityTimer->stop();
    } else {
        setOpacity(opacity - 0.05);
    }
}

DScrollBar::DScrollBar(QWidget *parent)
    : QScrollBar(parent)
    , DObject(*new DScrollBarPrivate(this))
{
    D_THEME_INIT_WIDGET(DScrollBar);

    d_func()->init();
}

bool DScrollBar::autoHide() const
{
    D_DC(DScrollBar);

    return d->autoHide;
}

void DScrollBar::setAutoHide(bool autoHide)
{
    D_D(DScrollBar);

    if (d->autoHide == autoHide)
        return;

    d->autoHide = autoHide;
    emit autoHideChanged(autoHide);

    update();
}

void DScrollBar::enterEvent(QEvent *event)
{
    D_D(DScrollBar);

    d->timer->stop();
    d->opacityTimer->stop();
    d->setOpacity(1);

    QScrollBar::enterEvent(event);
}

void DScrollBar::leaveEvent(QEvent *event)
{
    D_DC(DScrollBar);

    d->timer->start(1000);

    QScrollBar::leaveEvent(event);
}

void DScrollBar::paintEvent(QPaintEvent *event)
{
    D_D(DScrollBar);

    if (d->canPaint || !d->autoHide) {
        return QScrollBar::paintEvent(event);
    }

    QPixmap pixmap;
    QPainter painter(this);

    d->canPaint = true;

    setAttribute(Qt::WA_WState_InPaintEvent, false);
    pixmap = grab();

    painter.setOpacity(d->opacity);
    painter.drawPixmap(QPoint(0, 0), pixmap);
}

DWIDGET_END_NAMESPACE

#include "moc_dscrollbar.cpp"
