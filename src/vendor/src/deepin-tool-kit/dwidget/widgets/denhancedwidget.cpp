/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QEvent>
#include <QResizeEvent>

#include "denhancedwidget.h"

DWIDGET_BEGIN_NAMESPACE

class DEnhancedWidgetPrivate
{
    explicit DEnhancedWidgetPrivate(DEnhancedWidget *qq): q_ptr(qq) {}

    QSize old_size;
    QPoint old_pos;
    QWidget *target = NULL;
    bool enabled = true;

    DEnhancedWidget *q_ptr;

    Q_DECLARE_PUBLIC(DEnhancedWidget)
};

DEnhancedWidget::DEnhancedWidget(QWidget *w, QObject *parent):
    QObject(parent),
    d_ptr(new DEnhancedWidgetPrivate(this))
{
    if (w) {
        Q_D(DEnhancedWidget);

        d->target = w;
        w->installEventFilter(this);
    }
}

DEnhancedWidget::~DEnhancedWidget()
{
    delete d_ptr;
}

QWidget *DEnhancedWidget::target() const
{
    Q_D(const DEnhancedWidget);

    return d->target;
}

bool DEnhancedWidget::enabled() const
{
    Q_D(const DEnhancedWidget);

    return d->enabled;
}

void DEnhancedWidget::setTarget(QWidget *target)
{
    Q_D(DEnhancedWidget);

    if (d->target == target) {
        return;
    }

    if (d->enabled && d->target) {
        d->target->removeEventFilter(this);
    }
    if (d->enabled && target) {
        target->installEventFilter(this);
    }
    d->target = target;
    emit targetChanged(target);
}

void DEnhancedWidget::setEnabled(bool enabled)
{
    Q_D(DEnhancedWidget);

    if (d->enabled == enabled) {
        return;
    }

    if (d->target) {
        if (enabled) {
            d->target->installEventFilter(this);
        } else {
            d->target->removeEventFilter(this);
        }
    }

    d->enabled = enabled;
    emit enabledChanged(enabled);
}

bool DEnhancedWidget::eventFilter(QObject *o, QEvent *e)
{
    Q_D(DEnhancedWidget);

    if (o == d->target) {
        if (e->type() == QEvent::Resize) {
            QResizeEvent *event = static_cast<QResizeEvent *>(e);
            if (event) {
                QSize size = event->size();

                if (size.width() != d->old_size.width()) {
                    emit widthChanged(size.width());
                }

                if (size.height() != d->old_size.height()) {
                    emit heightChanged(size.height());
                }

                if (size != d->old_size) {
                    emit sizeChanged(size);
                }

                d->old_size = size;
            }
        } else if (e->type() == QEvent::Move) {
            QMoveEvent *event = static_cast<QMoveEvent *>(e);

            if (event) {
                QPoint pos = event->pos();

                if (pos.x() != d->old_pos.x()) {
                    emit xChanged(pos.x());
                }

                if (pos.y() != d->old_pos.y()) {
                    emit yChanged(pos.y());
                }

                if (pos != d->old_pos) {
                    emit positionChanged(pos);
                }

                d->old_pos = pos;
            }
        }
    }

    return false;
}

DEnhancedWidget::DEnhancedWidget(DEnhancedWidgetPrivate *dd, QWidget *w, QObject *parent):
    QObject(parent),
    d_ptr(dd)
{
    if (w) {
        w->installEventFilter(this);
    }
}

DWIDGET_END_NAMESPACE
