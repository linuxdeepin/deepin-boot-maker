/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QWidget>
#include <QVBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>

#include "dthememanager.h"
#include "dlistwidget.h"
#include "private/dlistwidget_p.h"

DWIDGET_BEGIN_NAMESPACE

DListWidgetPrivate::DListWidgetPrivate(DListWidget *qq):
        DScrollAreaPrivate(qq),
        itemWidth(-1),
        itemHeight(-1),
        checkMode(DListWidget::Radio),
        mainWidget(new DBoxWidget(QBoxLayout::TopToBottom)),
        visibleCount(0),
        checkable(false),
        toggleable(false),
        enableHorizontalScroll(false),
        enableVerticalScroll(false)
{

}

DListWidgetPrivate::~DListWidgetPrivate()
{

}

void DListWidgetPrivate::init()
{
    Q_Q(DListWidget);

    mainWidget->setObjectName("MainWidget");

    q->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    q->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    q->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    q->setAlignment(Qt::AlignHCenter);
    q->setFrameStyle(QFrame::NoFrame);
    q->setFrameShape(QFrame::NoFrame);
    q->setWidget(mainWidget);

    QObject::connect(mainWidget, &DBoxWidget::sizeChanged, q, &DListWidget::updateSize);
}

DListWidget::DListWidget(CheckMode checkMode, QWidget *parent) :
    DScrollArea(*new DListWidgetPrivate(this), parent)
{
    D_THEME_INIT_WIDGET(DListWidget);

    d_func()->checkMode = checkMode;
    d_func()->init();
}

DListWidget::DListWidget(QBoxLayout::Direction direction, DListWidget::CheckMode checkMode, QWidget *parent) :
    DScrollArea(*new DListWidgetPrivate(this), parent)
{
    D_THEME_INIT_WIDGET(DListWidget);

    d_func()->mainWidget->setDirection(direction);
    d_func()->checkMode = checkMode;
    d_func()->init();
}

int DListWidget::addWidget(QWidget *w, Qt::Alignment a)
{
    insertWidget(-1, w, a);

    return count()-1;
}

void DListWidget::addWidgets(const QList<QWidget*> &ws, Qt::Alignment a)
{
    insertWidgets(-1, ws, a);
}

void DListWidget::insertWidget(int index, QWidget *w, Qt::Alignment a)
{
    Q_D(DListWidget);

    if(w==NULL || d->widgetList.contains(w))
        return;

    if(d->itemWidth>0){
        w->setFixedWidth(d->itemWidth);
    }

    if(d->itemHeight>0){
        w->setFixedHeight(d->itemHeight);
    }

    if(index < 0) {
        d->widgetList << w;
        d->mainWidget->layout()->addWidget(w, 0, a);
    } else {
        d->widgetList.insert(index, w);
        d->mainWidget->layout()->insertWidget(index, w, 0, a);
    }

    d->mapVisible[w] = true;

    setVisibleCount(d->visibleCount + 1);

    emit countChanged(count());
}

void DListWidget::insertWidgets(int index, const QList<QWidget*> &ws, Qt::Alignment a)
{
    if(index < 0) {
        foreach (QWidget *w, ws) {
            insertWidget(-1, w, a);
        }
    } else {
        foreach (QWidget *w, ws) {
            insertWidget(index++, w, a);
        }
    }
}

void DListWidget::setItemSize(int w, int h)
{
    Q_D(DListWidget);

    d->itemWidth = w;
    d->itemHeight = h;

    if(d->itemWidth <= 0 && d->itemHeight <= 0)
        return;

    foreach (QWidget *w, d->widgetList) {
        if(d->itemWidth>0)
            w->setFixedWidth(d->itemWidth);
        if(d->itemHeight>0){
            w->setFixedHeight(d->itemHeight);
        }
    }
}

void DListWidget::clear(bool isDelete)
{
    Q_D(DListWidget);

    for(int i=0;i<count();++i){
        delete d->mainWidget->layout()->takeAt(i);
        d->widgetList[i]->setParent(NULL);
        if(isDelete)
            d->widgetList[i]->deleteLater();
    }

    d->mapVisible.clear();
    d->widgetList.clear();
    d->checkedList.clear();

    resize(0, 0);
    setVisibleCount(0);

    emit countChanged(count());
}

void DListWidget::removeWidget(int index, bool isDelete)
{
    Q_D(DListWidget);

    QWidget *w = getWidget(index);

    d->widgetList.removeAt(index);
    delete d->mainWidget->layout()->takeAt(index);
    d->checkedList.removeOne(index);

    if(d->mapVisible.value(w, false)){
        setVisibleCount(d->visibleCount -1);
    }
    d->mapVisible.remove(w);

    w->setParent(NULL);
    if(isDelete)
        w->deleteLater();

    emit countChanged(count());
}

void DListWidget::showWidget(int index)
{
    Q_D(DListWidget);

    QWidget *w = getWidget(index);

    if(!d->mapVisible.value(w, true)){
        w->show();
        d->mapVisible[w] = true;
        setVisibleCount(d->visibleCount+1);
    }
}

void DListWidget::hideWidget(int index)
{
    Q_D(DListWidget);

    QWidget *w = getWidget(index);

    if(d->mapVisible.value(w, false)){
        w->hide();
        d->mapVisible[w] = false;
        setVisibleCount(d->visibleCount-1);
    }
}

void DListWidget::setChecked(int index, bool checked)
{
    Q_D(DListWidget);

    if (!d->checkable || (checked && isChecked(index)) || (!checked && !isChecked(index)))
        return;

    if(d->checkMode == Radio && checked){
        int checkedWidget = firstChecked();
        if(checkedWidget>=0){
            getWidget(checkedWidget)->setProperty("checked", false);
            d->checkedList.removeOne(checkedWidget);
        }
    }

    if(index>=0 && index < count()){
        QWidget *w = getWidget(index);
        w->setProperty("checked", checked);
    }

    if(checked){
        d->checkedList << index;
    }else{
        d->checkedList.removeOne(index);
    }

    if(d->checkMode == Radio && checked){
        emit firstCheckedChanged(index);
    }

    emit checkedChanged(index, checked);
}

void DListWidget::setCheckMode(DListWidget::CheckMode checkMode)
{
    Q_D(DListWidget);

    d->checkMode = checkMode;
}

void DListWidget::setCheckable(bool checkable)
{
    Q_D(DListWidget);

    if (d->checkable == checkable)
        return;

    d->checkable = checkable;
}

void DListWidget::setToggleable(bool enableUncheck)
{
    Q_D(DListWidget);

    if (d->toggleable == enableUncheck)
        return;

    d->toggleable = enableUncheck;
    emit toggleableChanged(enableUncheck);
}

void DListWidget::setEnableHorizontalScroll(bool enableHorizontalScroll)
{
    Q_D(DListWidget);

    if (d->enableHorizontalScroll == enableHorizontalScroll)
        return;

    d->enableHorizontalScroll = enableHorizontalScroll;

    updateSize();

    emit enableHorizontalScrollChanged(enableHorizontalScroll);
}

void DListWidget::setEnableVerticalScroll(bool enableVerticalScroll)
{
    Q_D(DListWidget);

    if (d->enableVerticalScroll == enableVerticalScroll)
        return;

    d->enableVerticalScroll = enableVerticalScroll;

    updateSize();

    emit enableVerticalScrollChanged(enableVerticalScroll);
}

void DListWidget::setSpacing(int spacing)
{
    Q_D(DListWidget);

    d->mainWidget->layout()->setSpacing(spacing);
}

void DListWidget::setDirection(QBoxLayout::Direction direction)
{
    Q_D(DListWidget);

    d->mainWidget->layout()->setDirection(direction);
}

int DListWidget::count() const
{
    Q_D(const DListWidget);

    return d->widgetList.count();
}

int DListWidget::indexOf(const QWidget* w)
{
    Q_D(DListWidget);

    return d->widgetList.indexOf(const_cast<QWidget*>(w));
}

QWidget *DListWidget::getWidget(int index) const
{
    Q_D(const DListWidget);

    return d->widgetList[index];
}

int DListWidget::firstChecked() const
{
    Q_D(const DListWidget);

    return d->checkedList.count()>0 ? d->checkedList.first() : -1;
}

QList<int> DListWidget::checkedList() const
{
    Q_D(const DListWidget);

    return d->checkedList;
}

bool DListWidget::checkable() const
{
    Q_D(const DListWidget);

    return d->checkable;
}

bool DListWidget::toggleable() const
{
    Q_D(const DListWidget);

    return d->toggleable;
}

bool DListWidget::isChecked(int index) const
{
    Q_D(const DListWidget);

    return d->checkedList.contains(index);
}

int DListWidget::visibleCount() const
{
    Q_D(const DListWidget);

    return d->visibleCount;
}

int DListWidget::spacing() const
{
    Q_D(const DListWidget);

    return d->mainWidget->layout()->spacing();
}

QBoxLayout::Direction DListWidget::direction() const
{
    Q_D(const DListWidget);

    return d->mainWidget->direction();
}

DListWidget::CheckMode DListWidget::checkMode() const
{
    Q_D(const DListWidget);

    return d->checkMode;
}

void DListWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if(!checkable()){
        return DScrollArea::mouseReleaseEvent(e);
    }

    Q_D(DListWidget);

    QWidget *widget = childAt(e->pos());

    while(widget && widget->parentWidget() != d->mainWidget) {
        widget = widget->parentWidget();
    }

    int index = indexOf(widget);
    if(index>=0){
        if(toggleable())
            setChecked(index, !isChecked(index));
        else
            setChecked(index, true);

        emit clicked(index);
    }
}

void DListWidget::setVisibleCount(int count)
{
    Q_D(DListWidget);

    if(d->visibleCount == count)
        return;

    d->visibleCount = count;

    emit visibleCountChanged(count);
}

void DListWidget::updateSize()
{
    Q_D(DListWidget);

    if(!d->enableHorizontalScroll) {
        if(direction() == QBoxLayout::LeftToRight || direction() == QBoxLayout::RightToLeft)
            setFixedWidth(d->mainWidget->width());
        else
            setMinimumWidth(d->mainWidget->width());
    } else {
        setMinimumWidth(qMin(d->mainWidget->width(), maximumWidth()));
    }

    if(!d->enableVerticalScroll) {
        if(direction() == QBoxLayout::TopToBottom || direction() == QBoxLayout::BottomToTop)
            setFixedHeight(d->mainWidget->height());
        else
            setMinimumHeight(d->mainWidget->height());
    } else {
        setMinimumHeight(qMin(d->mainWidget->height(), maximumHeight()));
    }
}

QList<QWidget*> DListWidget::widgetList() const
{
    Q_D(const DListWidget);

    return d->widgetList;
}

QSize DListWidget::itemSize() const
{
    Q_D(const DListWidget);

    return QSize(d->itemWidth, d->itemHeight);
}

bool DListWidget::enableHorizontalScroll() const
{
    Q_D(const DListWidget);

    return d->enableHorizontalScroll;
}

bool DListWidget::enableVerticalScroll() const
{
    Q_D(const DListWidget);

    return d->enableVerticalScroll;
}

void DListWidget::setContentsMargins(int left, int top, int right, int bottom)
{
    Q_D(DListWidget);

    d->mainWidget->layout()->setContentsMargins(left, top, right, bottom);
}

void DListWidget::setContentsMargins(const QMargins &margins)
{
    Q_D(DListWidget);

    d->mainWidget->layout()->setContentsMargins(margins);
}

void DListWidget::getContentsMargins(int *left, int *top, int *right, int *bottom) const
{
    Q_D(const DListWidget);

    d->mainWidget->layout()->getContentsMargins(left, top, right, bottom);
}

QMargins DListWidget::contentsMargins() const
{
    Q_D(const DListWidget);

    return d->mainWidget->layout()->contentsMargins();
}

QRect DListWidget::contentsRect() const
{
    Q_D(const DListWidget);

    return d->mainWidget->layout()->contentsRect();
}

QSize DListWidget::sizeHint() const
{
    return minimumSize();
}

DListWidget::DListWidget(DListWidgetPrivate &dd, QWidget *parent):
    DScrollArea(dd, parent)
{
    D_THEME_INIT_WIDGET(DListWidget);

    d_func()->init();
}

DWIDGET_END_NAMESPACE
