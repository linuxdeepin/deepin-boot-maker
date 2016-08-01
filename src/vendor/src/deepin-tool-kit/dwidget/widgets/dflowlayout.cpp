/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QStyle>
#include <QDebug>

#include "dflowlayout.h"
#include "private/dflowlayout_p.h"

DWIDGET_BEGIN_NAMESPACE

DFlowLayoutPrivate::DFlowLayoutPrivate(DFlowLayout *qq) :
    DObjectPrivate(qq)
{

}

QSize DFlowLayoutPrivate::doLayout(const QRect &rect, bool testOnly) const
{
    D_QC(DFlowLayout);

    if(!testOnly) {
        if(!q->parentWidget() || q->parentWidget()->isHidden())
            return QSize();
    } else if(rect.width() <= 0){
        return QSize();
    }

    int left, top, right, bottom;

    q->getContentsMargins(&left, &top, &right, &bottom);
    QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);

    int x = q->parentWidget()->layoutDirection() == Qt::RightToLeft
            ? effectiveRect.right()
            :effectiveRect.x();
    int y = effectiveRect.y();

    QSize size_hint;

    if(flow == DFlowLayout::Flow::LeftToRight) {
        int maxWidth = 0;
        int lineHeight = 0;

        if(q->parentWidget()->layoutDirection() == Qt::RightToLeft) {
            for (QLayoutItem *item : itemList) {
                int nextX = x - item->sizeHint().width() - horizontalSpacing;

                if (nextX + horizontalSpacing < effectiveRect.x() && lineHeight > 0) {
                    maxWidth = qMax(effectiveRect.right() - x, maxWidth);
                    x = effectiveRect.right();
                    y = y + lineHeight + verticalSpacing;
                    nextX = x - item->sizeHint().width() - horizontalSpacing;
                    lineHeight = 0;
                }

                if (!testOnly) {
                    QRect item_geometry;

                    item_geometry.setSize(item->sizeHint());
                    item_geometry.moveTopRight(QPoint(x, y));
                    item->setGeometry(item_geometry);
                }

                x = nextX;
                lineHeight = qMax(lineHeight, item->sizeHint().height());
            }

            size_hint = QSize(maxWidth, y + lineHeight - rect.y() + bottom);
        } else {
            for (QLayoutItem *item : itemList) {
                int nextX = x + item->sizeHint().width() + horizontalSpacing;

                if (nextX - horizontalSpacing > effectiveRect.right() && lineHeight > 0) {
                    maxWidth = qMax(x, maxWidth);
                    x = effectiveRect.x();
                    y = y + lineHeight + verticalSpacing;
                    nextX = x + item->sizeHint().width() + horizontalSpacing;
                    lineHeight = 0;
                }

                if (!testOnly)
                    item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));

                x = nextX;
                lineHeight = qMax(lineHeight, item->sizeHint().height());
            }

            size_hint = QSize(maxWidth, y + lineHeight - rect.y() + bottom);
        }
    } else {
        int maxHeight = 0;
        int lineWidth = 0;

        if(q->parentWidget()->layoutDirection() == Qt::RightToLeft) {
            for (QLayoutItem *item : itemList) {
                int nextY = y + item->sizeHint().height() + verticalSpacing;

                if(nextY - verticalSpacing > effectiveRect.bottom() && lineWidth > 0) {
                    maxHeight = qMax(y, maxHeight);
                    y = effectiveRect.y();
                    x = x - lineWidth - horizontalSpacing;
                    nextY = y + item->sizeHint().height() + verticalSpacing;
                    lineWidth = 0;
                }

                if (!testOnly)
                    item->setGeometry(QRect(QPoint(x - item->sizeHint().width(), y), item->sizeHint()));

                y = nextY;
                lineWidth = qMax(lineWidth, item->sizeHint().width());
            }

            size_hint = QSize(rect.right() - x + lineWidth + right + 1, maxHeight);
        } else {
            for (QLayoutItem *item : itemList) {
                int nextY = y + item->sizeHint().height() + verticalSpacing;

                if(nextY - verticalSpacing > effectiveRect.bottom() && lineWidth > 0) {
                    maxHeight = qMax(y, maxHeight);
                    y = effectiveRect.y();
                    x = x + lineWidth + horizontalSpacing;
                    nextY = y + item->sizeHint().height() + verticalSpacing;
                    lineWidth = 0;
                }

                if (!testOnly)
                    item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));

                y = nextY;
                lineWidth = qMax(lineWidth, item->sizeHint().width());
            }

            size_hint = QSize(x + lineWidth - rect.x() + right, maxHeight);
        }
    }

    if(!testOnly) {
        if(sizeHint != size_hint) {
            sizeHint = size_hint;
            Q_EMIT q->sizeHintChanged(sizeHint);
        }
    }

    return size_hint;
}

DFlowLayout::DFlowLayout(QWidget *parent) :
    QLayout(parent),
    DObject(*new DFlowLayoutPrivate(this))
{

}

DFlowLayout::DFlowLayout() :
    QLayout(),
    DObject(*new DFlowLayoutPrivate(this))
{

}

DFlowLayout::~DFlowLayout()
{
    QLayoutItem *item;

    while ((item = takeAt(0)))
        delete item;
}

void DFlowLayout::insertItem(int index, QLayoutItem *item)
{
    d_func()->itemList.insert(index, item);

    emit countChanged(count());
}

void DFlowLayout::insertWidget(int index, QWidget *widget)
{
    addChildWidget(widget);
    insertItem(index, new QWidgetItemV2(widget));
}

void DFlowLayout::insertLayout(int index, QLayout *layout)
{
    addChildLayout(layout);
    insertItem(index, layout);
}

void DFlowLayout::insertSpacing(int index, int size)
{
    QSpacerItem *b;

    b = new QSpacerItem(size, 0, QSizePolicy::Fixed, QSizePolicy::Minimum);

    insertSpacerItem(index, b);
}

void DFlowLayout::insertStretch(int index, int stretch)
{
    Q_UNUSED(stretch);

    QSpacerItem *b;

    b = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    insertSpacerItem(index, b);
}

void DFlowLayout::insertSpacerItem(int index, QSpacerItem *spacerItem)
{
    insertItem(index, spacerItem);
}

void DFlowLayout::addSpacing(int size)
{
    insertSpacing(count(), size);
}

void DFlowLayout::addStretch(int stretch)
{
    insertStretch(count(), stretch);
}

void DFlowLayout::addSpacerItem(QSpacerItem *spacerItem)
{
    insertSpacerItem(count(), spacerItem);
}

void DFlowLayout::addItem(QLayoutItem *item)
{
    insertItem(count(), item);
}

bool DFlowLayout::hasHeightForWidth() const
{
    D_DC(DFlowLayout);

    return d->flow == DFlowLayout::Flow::LeftToRight;
}

int DFlowLayout::heightForWidth(int width) const
{
    D_DC(DFlowLayout);

    QWidget *parentWidget = this->parentWidget();

    if(parentWidget && width == parentWidget->width()) {
        return d->sizeHint.height();
    }

    return d->doLayout(QRect(0, 0, width, 0), true).height();
}

int DFlowLayout::count() const
{
    return d_func()->itemList.count();
}

QLayoutItem *DFlowLayout::itemAt(int index) const
{
    return d_func()->itemList.value(index);
}

QSize DFlowLayout::minimumSize() const
{
    D_DC(DFlowLayout);

    QSize size;

    for (QLayoutItem *item : d->itemList)
        size = size.expandedTo(item->minimumSize());

    size += QSize(2 * margin(), 2 * margin());

    return size;
}

void DFlowLayout::setGeometry(const QRect &rect)
{
    if(rect == geometry())
        return;

    QLayout::setGeometry(QRect(rect.topLeft(),
                               d_func()->doLayout(rect, false)));
}

QSize DFlowLayout::sizeHint() const
{
    return d_func()->sizeHint;
}

QLayoutItem *DFlowLayout::takeAt(int index)
{
    D_D(DFlowLayout);

    if (index < 0 || index >= d->itemList.count()) {
        return 0;
    }

    QLayoutItem *item = d->itemList.takeAt(index);

    if (QLayout *l = item->layout()) {
        // sanity check in case the user passed something weird to QObject::setParent()
        if (l->parent() == this)
            l->setParent(0);
    }

    emit countChanged(count());

    return item;
}

Qt::Orientations DFlowLayout::expandingDirections() const
{
    return Qt::Vertical;
}

int DFlowLayout::horizontalSpacing() const
{
    return d_func()->horizontalSpacing;
}

int DFlowLayout::verticalSpacing() const
{
    return d_func()->verticalSpacing;
}

DFlowLayout::Flow DFlowLayout::flow() const
{
    D_DC(DFlowLayout);

    return d->flow;
}

void DFlowLayout::setHorizontalSpacing(int horizontalSpacing)
{
    D_D(DFlowLayout);

    if(horizontalSpacing == d->horizontalSpacing)
        return;

    d->horizontalSpacing = horizontalSpacing;

    emit horizontalSpacingChanged(horizontalSpacing);

    invalidate();
}

void DFlowLayout::setVerticalSpacing(int verticalSpacing)
{
    D_D(DFlowLayout);

    if(verticalSpacing == d->verticalSpacing)
        return;

    d->verticalSpacing = verticalSpacing;

    emit verticalSpacingChanged(verticalSpacing);

    invalidate();
}

void DFlowLayout::setSpacing(int spacing)
{
    setHorizontalSpacing(spacing);
    setVerticalSpacing(spacing);
}

void DFlowLayout::setFlow(Flow direction)
{
    D_D(DFlowLayout);

    if (d->flow == direction)
        return;

    d->flow = direction;
    emit flowChanged(direction);

    invalidate();
}

DWIDGET_END_NAMESPACE
