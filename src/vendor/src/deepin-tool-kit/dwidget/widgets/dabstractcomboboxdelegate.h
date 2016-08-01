/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DABSTRACTCOMBOBOXDELEGATE_H
#define DABSTRACTCOMBOBOXDELEGATE_H

#include <QObject>
#include <QPainter>
#include <QStyledItemDelegate>

#include "dcombobox.h"

DWIDGET_BEGIN_NAMESPACE

class DAbstractComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DAbstractComboBoxDelegate(QObject *parent = 0);
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter * painter,const QStyleOptionViewItem & option,const QModelIndex & index) const;
//    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

};

DWIDGET_END_NAMESPACE

#endif // DABSTRACTCOMBOBOXDELEGATE_H
