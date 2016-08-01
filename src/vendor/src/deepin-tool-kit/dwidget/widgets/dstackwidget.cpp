/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QDebug>

#include "dthememanager.h"
#include "dstackwidget.h"
#include "private/dstackwidget_p.h"

DWIDGET_BEGIN_NAMESPACE

DAbstractStackWidgetTransitionPrivate::DAbstractStackWidgetTransitionPrivate(DAbstractStackWidgetTransition *qq):
    DObjectPrivate(qq),
    animation(new QVariantAnimation(qq))
{

}

DAbstractStackWidgetTransitionPrivate::~DAbstractStackWidgetTransitionPrivate()
{

}

void DAbstractStackWidgetTransitionPrivate::init()
{
    Q_Q(DAbstractStackWidgetTransition);

    QObject::connect(animation, &QVariantAnimation::valueChanged, q, &DAbstractStackWidgetTransition::updateVariant);
}

DAbstractStackWidgetTransition::DAbstractStackWidgetTransition(QObject *parent):
    QObject(parent),
    DObject(*new DAbstractStackWidgetTransitionPrivate(this))
{
    d_func()->init();
}

void DAbstractStackWidgetTransition::beginTransition(const TransitionInfo &info)
{
    Q_D(DAbstractStackWidgetTransition);

    d->info = info;
}

QVariantAnimation *DAbstractStackWidgetTransition::animation() const
{
    Q_D(const DAbstractStackWidgetTransition);

    return d->animation;
}

DAbstractStackWidgetTransition::DAbstractStackWidgetTransition(DAbstractStackWidgetTransitionPrivate &dd,
                                                               QObject *parent):
    QObject(parent),
    DObject(dd)
{
    d_func()->init();
}

const DAbstractStackWidgetTransition::TransitionInfo &DAbstractStackWidgetTransition::info() const
{
    Q_D(const DAbstractStackWidgetTransition);

    return d->info;
}

DSlideStackWidgetTransition::DSlideStackWidgetTransition(QObject *parent):
    DAbstractStackWidgetTransition(parent)
{

}

void DSlideStackWidgetTransition::beginTransition(const TransitionInfo &info)
{
    DAbstractStackWidgetTransition::beginTransition(info);

    info.oldWidget->move(0, 0);
    if(info.type == Push) {
        info.newWidget->move(info.stackWidget->width(), 0);
    } else if(info.type == Pop) {
        info.newWidget->move(-info.stackWidget->width(), 0);
    }

    info.oldWidget->show();
    info.newWidget->show();

    animation()->setStartValue(0);
    animation()->setEndValue(-info.newWidget->x());
    animation()->start();
}

void DSlideStackWidgetTransition::updateVariant(const QVariant &variant)
{
    int x = variant.toInt();

    info().oldWidget->move(x, 0);

    if(info().type == Push) {
        info().newWidget->move(info().stackWidget->width() + x, 0);
    } else if(info().type == Pop) {
        info().newWidget->move(x - info().stackWidget->width(), 0);
    }
}

DStackWidgetPrivate::DStackWidgetPrivate(DStackWidget *qq):
    DObjectPrivate(qq)
{

}

DStackWidgetPrivate::~DStackWidgetPrivate()
{

}

void DStackWidgetPrivate::init()
{
    Q_Q(DStackWidget);

    currentIndex = -1;
    currentWidget = nullptr;
    transition = nullptr;
    q->setTransition(new DSlideStackWidgetTransition(q));
}

void DStackWidgetPrivate::setCurrentIndex(int index)
{
    if(index == currentIndex)
        return;

    Q_Q(DStackWidget);

    if(index >= 0 && index < widgetList.count()){
        currentIndex = index;
        currentWidget = widgetList[index];
    } else {
        currentIndex = -1;
        currentWidget = nullptr;
    }

    q->currentIndexChanged(index);
    q->currentWidgetChanged(currentWidget);
}

DStackWidget::DStackWidget(QWidget *parent) :
    QWidget(parent),
    DObject(*new DStackWidgetPrivate(this))
{
    D_THEME_INIT_WIDGET(DStackWidget)

    d_func()->init();
}

bool DStackWidget::busy() const
{
    Q_D(const DStackWidget);

    return d->transition->animation()->state() != QVariantAnimation::Stopped;
}

int DStackWidget::depth() const
{
    Q_D(const DStackWidget);

    return d->widgetList.count();
}

int DStackWidget::currentIndex() const
{
    Q_D(const DStackWidget);

    return d->currentIndex;
}

QWidget *DStackWidget::currentWidget() const
{
    Q_D(const DStackWidget);

    return d->currentWidget;
}

DAbstractStackWidgetTransition *DStackWidget::transition() const
{
    Q_D(const DStackWidget);

    return d->transition;
}

int DStackWidget::animationDuration() const
{
    Q_D(const DStackWidget);

    return d->transition->animation()->duration();
}

QEasingCurve::Type DStackWidget::animationType() const
{
    Q_D(const DStackWidget);

    return d->transition->animation()->easingCurve().type();
}

int DStackWidget::pushWidget(QWidget *widget, bool enableTransition)
{
    insertWidget(depth(), widget, enableTransition);

    return depth() - 1;
}

void DStackWidget::insertWidget(int depth, QWidget *widget, bool enableTransition)
{
    Q_D(DStackWidget);

    widget->setParent(this);
    d->widgetList.insert(depth, widget);

    if(depth == this->depth() - 1)
        setCurrentIndex(depth, DAbstractStackWidgetTransition::Push, enableTransition);
    else
        d->setCurrentIndex(indexOf(currentWidget()));
}

void DStackWidget::popWidget(QWidget *widget, bool isDelete, int count, bool enableTransition)
{
    Q_D(DStackWidget);

    int i = widget ? indexOf(widget) : currentIndex();

    if(i < 0 || i >= depth())
        return;

    bool current_widget_deleted = false;

    while(count-- > 0){
        QWidget *tmp_widget = d->widgetList[i];

        if(tmp_widget == currentWidget()) {
            current_widget_deleted = true;
        } else if(isDelete) {
            tmp_widget->deleteLater();
        }

        d->widgetList.removeAt(i);
    }

    if(current_widget_deleted && isDelete){
        if(enableTransition && depth()){
            d->trashWidgetList << d->currentWidget;
        } else if(d->currentWidget) {
            d->currentWidget->deleteLater();
            d->currentWidget = nullptr;
        }
    }

    setCurrentIndex(depth() - 1, DAbstractStackWidgetTransition::Pop, enableTransition && current_widget_deleted);
}

void DStackWidget::clear()
{
    Q_D(DStackWidget);

    qDeleteAll(d->widgetList.begin(), d->widgetList.end());
    d->widgetList.clear();
    d->setCurrentIndex(-1);
}

int DStackWidget::indexOf(QWidget *widget) const
{
    Q_D(const DStackWidget);

    return d->widgetList.indexOf(widget);
}

QWidget *DStackWidget::getWidgetByIndex(int index) const
{
    Q_D(const DStackWidget);

    return d->widgetList[index];
}

void DStackWidget::setCurrentIndex(int currentIndex, DAbstractStackWidgetTransition::TransitionType type,
                                   bool enableTransition)
{
    Q_D(DStackWidget);

    if(enableTransition && currentWidget() && currentIndex >= 0) {
        DAbstractStackWidgetTransition::TransitionInfo info;
        info.stackWidget = this;
        info.oldWidget = currentWidget();
        info.newWidget = getWidgetByIndex(depth() - 1);
        info.type = type;

        d->setCurrentIndex(currentIndex);
        d->transition->beginTransition(info);
    } else {
        if(currentWidget()) {
            currentWidget()->hide();
        }

        d->setCurrentIndex(currentIndex);

        if(currentWidget()) {
            currentWidget()->move(0, 0);
            currentWidget()->show();
        }

        emit switchWidgetFinished();
    }
}

void DStackWidget::setCurrentWidget(QWidget *currentWidget, DAbstractStackWidgetTransition::TransitionType type,
                                    bool enableTransition)
{
    setCurrentIndex(indexOf(currentWidget), type, enableTransition);
}

void DStackWidget::setTransition(DAbstractStackWidgetTransition *transition)
{
    Q_D(DStackWidget);

    if(d->transition){
        d->transition->deleteLater();
    }

    transition->setParent(this);
    d->transition = transition;

    connect(transition->animation(), &QVariantAnimation::stateChanged,
            this, [this, d](QAbstractAnimation::State newState, QAbstractAnimation::State oldState){
        if(newState == QVariantAnimation::Stopped) {
            busyChanged(false);
            qDeleteAll(d->trashWidgetList);
            d->trashWidgetList.clear();

            emit switchWidgetFinished();
        } else if(oldState == QVariantAnimation::Stopped) {
            busyChanged(true);
        }
    });
}

void DStackWidget::setAnimationDuration(int animationDuration)
{
    Q_D(DStackWidget);

    d->transition->animation()->setDuration(animationDuration);
}

void DStackWidget::setAnimationType(QEasingCurve::Type animationType)
{
    Q_D(DStackWidget);

    d->transition->animation()->setEasingCurve(animationType);
}

DStackWidget::DStackWidget(DStackWidgetPrivate &dd, QWidget *parent):
    QWidget(parent),
    DObject(dd)
{
    D_THEME_INIT_WIDGET(DStackWidget)

    d_func()->init();
}

DWIDGET_END_NAMESPACE
