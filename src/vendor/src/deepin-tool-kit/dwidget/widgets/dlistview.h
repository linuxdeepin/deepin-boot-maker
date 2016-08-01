/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DLISTVIEW_H
#define DLISTVIEW_H

#include "dwidget_global.h"

#include <dobject.h>

#include <QStyledItemDelegate>
#include <QListView>

DWIDGET_BEGIN_NAMESPACE

class DVariantListModel : public QAbstractListModel
{
public:
    explicit DVariantListModel(QObject *parent = 0);

    int rowCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;

private:
    QList<QVariant> dataList;
};

class DListViewPrivate;
class LIBDTKWIDGETSHARED_EXPORT DListView : public QListView, public DObject
{
    Q_OBJECT

    /// item count.
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    /// list layout orientation
    Q_PROPERTY(Qt::Orientation orientation READ orientation NOTIFY orientationChanged)

public:
    explicit DListView(QWidget *parent = 0);

    State state() const;

    QWidget *getHeaderWidget(int index) const;
    QWidget *getFooterWidget(int index) const;

    /// return true if rect intersects contentsVisualRect+qMax(cacheBuffer,cacheCount)
    bool isActiveRect(const QRect &rect) const;
    bool isVisualRect(const QRect &rect) const;

    int count() const;

    Qt::Orientation orientation() const;

    void setModel(QAbstractItemModel * model) Q_DECL_OVERRIDE;

public Q_SLOTS:
    bool addItem(const QVariant &data);
    bool addItems(const QVariantList &datas);
    bool insertItem(int index, const QVariant &data);
    bool insertItems(int index, const QVariantList &datas);
    bool removeItem(int index);
    bool removeItems(int index, int count);

    int addHeaderWidget(QWidget *widget);
    void removeHeaderWidget(int index);
    QWidget *takeHeaderWidget(int index);
    int addFooterWidget(QWidget *widget);
    void removeFooterWidget(int index);
    QWidget *takeFooterWidget(int index);

    void setOrientation(QListView::Flow flow, bool wrapping);
    void edit(const QModelIndex &index);

Q_SIGNALS:
    void countChanged(int count) Q_DECL_DEPRECATED;
    void rowCountChanged();
    void orientationChanged(Qt::Orientation orientation);
    void currentChanged(const QModelIndex &previous);
    void triggerEdit(const QModelIndex &index);

protected:
#if(QT_VERSION < 0x050500)
    void setViewportMargins(int left, int top, int right, int bottom);
    void setViewportMargins(const QMargins &margins);
    QMargins viewportMargins() const;
#endif

    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void currentChanged(const QModelIndex & current, const QModelIndex & previous) Q_DECL_OVERRIDE;
    bool edit(const QModelIndex & index, EditTrigger trigger, QEvent * event) Q_DECL_OVERRIDE;

private:
    void setFlow(QListView::Flow flow);
    void setWrapping(bool enable);

    D_DECLARE_PRIVATE(DListView)
};

DWIDGET_END_NAMESPACE

#endif // DLISTVIEW_H
