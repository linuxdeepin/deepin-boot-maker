/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

/*!
    \class DBoxWidget
    \brief The DBoxWidget widget provides a Adaptive child widget size
    when child widget size changed.

    \inmodule libdui
*/

#include <QBoxLayout>
#include <QEvent>
#include <QDebug>
#include <QResizeEvent>

#include "dthememanager.h"
#include "dboxwidget.h"
#include "private/dboxwidget_p.h"

DWIDGET_BEGIN_NAMESPACE

DBoxWidgetPrivate::DBoxWidgetPrivate(DBoxWidget *qq):
    DObjectPrivate(qq),
    layout(new QBoxLayout(QBoxLayout::TopToBottom))
{

}

void DBoxWidgetPrivate::init()
{
    Q_Q(DBoxWidget);

    layout->setMargin(0);
    layout->setSpacing(0);

    q->setLayout(layout);
    q->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

DBoxWidget::DBoxWidget(QBoxLayout::Direction direction, QWidget *parent):
    QFrame(parent),
    DObject(*new DBoxWidgetPrivate(this))
{
    D_THEME_INIT_WIDGET(DBoxWidget);

    d_func()->layout->setDirection(direction);
    d_func()->init();
}

QBoxLayout::Direction DBoxWidget::direction() const
{
    Q_D(const DBoxWidget);

    return d->layout->direction();
}

QBoxLayout *DBoxWidget::layout() const
{
    Q_D(const DBoxWidget);

    return d->layout;
}

void DBoxWidget::addWidget(QWidget *widget)
{
    layout()->addWidget(widget);
}

void DBoxWidget::setDirection(QBoxLayout::Direction direction)
{
    Q_D(DBoxWidget);

    if (d->layout->direction() == direction)
        return;

    d->layout->setDirection(direction);
    emit directionChanged(direction);
}

void DBoxWidget::updateSize(const QSize &size)
{
    if(direction() == QBoxLayout::TopToBottom || QBoxLayout::BottomToTop) {
        setFixedHeight(size.height());

        if(size.width() > minimumWidth())
            setMinimumWidth(qMin(size.width(), maximumWidth()));
    } else {
        setFixedWidth(size.width());

        if(size.height() > minimumHeight())
            setMinimumHeight(qMin(size.height(), maximumHeight()));
    }
}

bool DBoxWidget::event(QEvent *ee)
{
    Q_D(const DBoxWidget);

    if(ee->type() == QEvent::LayoutRequest) {
        if(size() != d->layout->sizeHint()) {
            updateSize(d->layout->sizeHint());
            updateGeometry();
        }
    } else if(ee->type() == QEvent::Resize) {
        emit sizeChanged(size());
    } else if(ee->type() == QEvent::ChildAdded) {
        updateSize(d->layout->sizeHint());
    } else if(ee->type() == QEvent::ChildRemoved) {
        updateSize(d->layout->sizeHint());
    } else if(ee->type() == QEvent::Show) {
        updateSize(d->layout->sizeHint());
    }

    return QWidget::event(ee);
}

QSize DBoxWidget::sizeHint() const
{
    Q_D(const DBoxWidget);

    return d->layout->sizeHint();
}

DHBoxWidget::DHBoxWidget(QWidget *parent):
    DBoxWidget(QBoxLayout::LeftToRight, parent)
{

}

DVBoxWidget::DVBoxWidget(QWidget *parent):
    DBoxWidget(QBoxLayout::TopToBottom, parent)
{

}

DWIDGET_END_NAMESPACE
