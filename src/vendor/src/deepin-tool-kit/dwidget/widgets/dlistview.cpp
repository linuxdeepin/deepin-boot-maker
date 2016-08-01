/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QDebug>
#include <QScrollBar>

#include "dthememanager.h"
#include "dboxwidget.h"
#include "dlistview.h"
#include "private/dlistview_p.h"
#include "dflowlayout.h"

DWIDGET_BEGIN_NAMESPACE

DVariantListModel::DVariantListModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

int DVariantListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return dataList.count();
}

QVariant DVariantListModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role);

    return dataList.value(index.row());
}

bool DVariantListModel::setData(const QModelIndex &index,
                                 const QVariant &value, int role)
{
    if (index.row() >= 0 && index.row() < dataList.size()) {
        dataList.replace(index.row(), value);
        Q_EMIT dataChanged(index, index, QVector<int>() << role);

        return true;
    }

    return false;
}

bool DVariantListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (count < 1 || row < 0 || row > rowCount(parent))
        return false;

    beginInsertRows(QModelIndex(), row, row + count - 1);

    for (int r = 0; r < count; ++r)
        dataList.insert(row, QVariant());

    endInsertRows();

    return true;
}

bool DVariantListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (count <= 0 || row < 0 || (row + count) > rowCount(parent))
        return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for (int r = 0; r < count; ++r)
        dataList.removeAt(row);

    endRemoveRows();

    return true;
}

DListViewPrivate::DListViewPrivate(DListView *qq) :
    DObjectPrivate(qq)
{

}

DListViewPrivate::~DListViewPrivate()
{

}

void DListViewPrivate::init()
{
    D_Q(DListView);

    q->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    q->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
}

void DListViewPrivate::onOrientationChanged()
{
    D_Q(DListView);

    bool isVerticalLayout = (q->orientation() == Qt::Vertical);

    if(headerLayout) {
        headerLayout->setDirection(isVerticalLayout
                                   ? QBoxLayout::TopToBottom
                                   : QBoxLayout::LeftToRight);
        if(isVerticalLayout) {
            headerLayout->resize(q->width(), headerLayout->sizeHint().height());
        } else {
            headerLayout->resize(headerLayout->sizeHint().width(), q->height());
        }

        headerLayout->layout()->update();
    }

    if(footerLayout) {
        footerLayout->setDirection(isVerticalLayout
                                   ? QBoxLayout::TopToBottom
                                   : QBoxLayout::LeftToRight);

        if(isVerticalLayout) {
            footerLayout->resize(q->width(), footerLayout->sizeHint().height());
            footerLayout->move(0, q->height() - footerLayout->height());
        } else {
            footerLayout->resize(footerLayout->sizeHint().width(), q->height());
            footerLayout->move(q->width() - footerLayout->width(), 0);
        }

        footerLayout->layout()->update();
    }
}

DListView::DListView(QWidget *parent) :
    QListView(parent),
    DObject(*new DListViewPrivate(this))
{
    D_THEME_INIT_WIDGET(DListView);

    d_func()->init();
}

QAbstractItemView::State DListView::state() const
{
    return QListView::state();
}

void DListView::setFlow(QListView::Flow flow)
{
    QListView::setFlow(flow);
}

void DListView::setWrapping(bool enable)
{
    QListView::setWrapping(enable);
}

QWidget *DListView::getHeaderWidget(int index) const
{
    return d_func()->headerList.value(index);
}

QWidget *DListView::getFooterWidget(int index) const
{
    return d_func()->footerList.value(index);
}

bool DListView::isActiveRect(const QRect &rect) const
{
//    D_DC(DListView);

    QRect area = viewport()->geometry();

    return area.intersects(rect);
}

bool DListView::isVisualRect(const QRect &rect) const
{
    const QRect &area = viewport()->geometry();

    return area.intersects(rect);
}

int DListView::count() const
{
    return model()->rowCount(rootIndex());
}

Qt::Orientation DListView::orientation() const
{
    bool isVerticalLayout = isWrapping()
                            ? flow() == QListView::LeftToRight
                            : flow() == QListView::TopToBottom;

    return isVerticalLayout ? Qt::Vertical : Qt::Horizontal;
}

void DListView::setModel(QAbstractItemModel *model)
{
    QAbstractItemModel *old_model = this->model();

    if (old_model) {
        disconnect(old_model, &QAbstractItemModel::rowsInserted, this, &DListView::rowCountChanged);
        disconnect(old_model, &QAbstractItemModel::rowsRemoved, this, &DListView::rowCountChanged);
    }

    QListView::setModel(model);

    model = this->model();

    if (model) {
        connect(model, &QAbstractItemModel::rowsInserted, this, &DListView::rowCountChanged);
        connect(model, &QAbstractItemModel::rowsRemoved, this, &DListView::rowCountChanged);
    }
}

bool DListView::addItem(const QVariant &data)
{
    return insertItem(count(), data);
}

bool DListView::addItems(const QVariantList &datas)
{
    return insertItems(count(), datas);
}

bool DListView::insertItem(int index, const QVariant &data)
{
    if(!model()->insertRow(index))
        return false;

    return model()->setData(model()->index(index, 0, rootIndex()), data);
}

bool DListView::insertItems(int index, const QVariantList &datas)
{
    if(!model()->insertRows(index, datas.count()))
        return false;

    for(int i = 0; i < datas.count(); ++i)
        model()->setData(model()->index(index + i, 0, rootIndex()), datas.at(i));

    return true;
}

bool DListView::removeItem(int index)
{
    return model()->removeRow(index);
}

bool DListView::removeItems(int index, int count)
{
    return model()->removeRows(index, count);
}

int DListView::addHeaderWidget(QWidget *widget)
{
    D_D(DListView);

    int index = d->headerList.indexOf(widget);

    if(index >= 0)
        return index;

    if(!d->headerLayout) {
        bool isVerticalLayout = (orientation() == Qt::Vertical);

        d->headerLayout = new DBoxWidget(isVerticalLayout
                                         ? QBoxLayout::TopToBottom
                                         : QBoxLayout::LeftToRight, this);

        if(isVerticalLayout)
            d->headerLayout->resize(width(), d->headerLayout->height());
        else
            d->headerLayout->resize(d->headerLayout->width(), height());

        connect(d->headerLayout, &DBoxWidget::sizeChanged,
                this, [this](const QSize &size) {
            bool isVerticalLayout = (orientation() == Qt::Vertical);

            QMargins margins = this->viewportMargins();

            if(isVerticalLayout)
                margins.setTop(size.height());
            else
                margins.setLeft(size.width());

            setViewportMargins(margins);
        });

        d->headerLayout->show();
    }

    d->headerLayout->addWidget(widget);
    d->headerList << widget;

    return d->headerList.count() - 1;
}

void DListView::removeHeaderWidget(int index)
{
    QWidget *widget = takeHeaderWidget(index);

    if(widget)
        widget->deleteLater();
}

QWidget *DListView::takeHeaderWidget(int index)
{
    D_D(DListView);

    QWidget *widget = d->headerList.takeAt(index);

    d->headerLayout->layout()->removeWidget(widget);

    if(d->headerList.isEmpty()) {
        d->headerLayout->deleteLater();
        d->headerLayout = nullptr;
    }

    return widget;
}

int DListView::addFooterWidget(QWidget *widget)
{
    D_D(DListView);

    int index = d->footerList.indexOf(widget);

    if(index >= 0)
        return index;

    if(!d->footerLayout) {
        bool isVerticalLayout = (orientation() == Qt::Vertical);

        d->footerLayout = new DBoxWidget(isVerticalLayout
                                         ? QBoxLayout::TopToBottom
                                         : QBoxLayout::LeftToRight, this);

        if(isVerticalLayout)
            d->footerLayout->resize(width(), d->footerLayout->height());
        else
            d->footerLayout->resize(d->footerLayout->width(), height());

        connect(d->footerLayout, &DBoxWidget::sizeChanged,
                this, [this](const QSize &size) {
            bool isVerticalLayout = (orientation() == Qt::Vertical);

            QMargins margins = this->viewportMargins();

            D_D(DListView);

            if(isVerticalLayout) {
                margins.setBottom(size.height());
                d->footerLayout->move(0, height() - d->footerLayout->height());
            } else {
                margins.setRight(size.width());
                d->footerLayout->move(width() - d->footerLayout->width(), 0);
            }

            setViewportMargins(margins);
        });

        d->footerLayout->show();
    }

    d->footerLayout->addWidget(widget);
    d->footerList << widget;

    return d->footerList.count() - 1;
}

void DListView::removeFooterWidget(int index)
{
    QWidget *widget = takeFooterWidget(index);

    if(widget)
        widget->deleteLater();
}

QWidget *DListView::takeFooterWidget(int index)
{
    D_D(DListView);

    QWidget *widget = d->footerList.takeAt(index);

    d->footerLayout->layout()->removeWidget(widget);

    if(d->footerList.isEmpty()) {
        d->footerLayout->deleteLater();
        d->footerLayout = nullptr;
    }

    return widget;
}

void DListView::setOrientation(QListView::Flow flow, bool wrapping)
{
    Qt::Orientation old_orientation = this->orientation();

    setFlow(flow);
    setWrapping(wrapping);

    Qt::Orientation orientation = this->orientation();

    if(old_orientation != orientation) {
        D_D(DListView);

        QMargins margins = viewportMargins();

        if(old_orientation == Qt::Vertical) {
            if(d->headerLayout)
                margins.setTop(0);

            if(d->footerLayout)
                margins.setBottom(0);
        } else {
            if(d->headerLayout)
                margins.setLeft(0);

            if(d->footerLayout)
                margins.setRight(0);
        }

        setViewportMargins(margins);

        d->onOrientationChanged();

        emit orientationChanged(orientation);
    }
}

void DListView::edit(const QModelIndex &index)
{
    QListView::edit(index);
}

#if(QT_VERSION < 0x050500)
void DListView::setViewportMargins(int left, int top, int right, int bottom)
{
    D_D(DListView);

    d->left = left;
    d->top = top;
    d->right = right;
    d->bottom = bottom;

    QListView::setViewportMargins(left, top, right, bottom);
}

void DListView::setViewportMargins(const QMargins &margins)
{
    setViewportMargins(margins.left(), margins.top(), margins.right(), margins.bottom());
}

QMargins DListView::viewportMargins() const
{
    D_DC(DListView);

    return QMargins(d->left, d->top, d->right, d->bottom);
}
#endif

void DListView::resizeEvent(QResizeEvent *event)
{
    QListView::resizeEvent(event);

    D_D(DListView);

    bool isVerticalLayout = (orientation() == Qt::Vertical);

    if(d->headerLayout) {
        if(isVerticalLayout)
            d->headerLayout->resize(width(), d->headerLayout->height());
        else
            d->headerLayout->resize(d->headerLayout->width(), height());
    }

    if(d->footerLayout) {
        if(isVerticalLayout) {
            d->footerLayout->resize(width(), d->footerLayout->height());
            d->footerLayout->move(0, height() - d->footerLayout->height());
        } else {
            d->footerLayout->resize(d->footerLayout->width(), height());
            d->footerLayout->move(width() - d->footerLayout->width(), 0);
        }
    }
}

void DListView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    QListView::currentChanged(current, previous);

    emit currentChanged(previous);
}

bool DListView::edit(const QModelIndex &index, QAbstractItemView::EditTrigger trigger, QEvent *event)
{
    if (QWidget *w = indexWidget(index)) {
        Qt::ItemFlags flags = model()->flags(index);

        if (((flags & Qt::ItemIsEditable) == 0) || ((flags & Qt::ItemIsEnabled) == 0))
            return false;
        if (state() == QAbstractItemView::EditingState)
            return false;
        if (trigger == QAbstractItemView::AllEditTriggers) // force editing
            return true;
        if ((trigger & editTriggers()) == QAbstractItemView::SelectedClicked
            && !selectionModel()->isSelected(index))
            return false;

        if(trigger & editTriggers()) {
            w->setFocus();

            Q_EMIT triggerEdit(index);

            return true;
        }
    }

    bool tmp = QListView::edit(index, trigger, event);

    if(tmp)
        Q_EMIT triggerEdit(index);

    return tmp;
}

#include "moc_dlistview.cpp"

DWIDGET_END_NAMESPACE
