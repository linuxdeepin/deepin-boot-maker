/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DCOLORCOMBOBOX_H
#define DCOLORCOMBOBOX_H

#include <QLabel>
#include <QObject>
#include <QPainter>
#include <QPaintEvent>
#include <QJsonObject>
#include <QStyleOptionViewItem>
#include <QStyledItemDelegate>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QColor>

#include "dwidget_global.h"
#include "dcomboboxmodel.h"
#include "dcombobox.h"
#include "dabstractcomboboxdelegate.h"

DWIDGET_BEGIN_NAMESPACE

class DColorComboBoxPrivate;
class LIBDTKWIDGETSHARED_EXPORT DColorComboBox : public DComboBox
{
    Q_OBJECT
public:
    explicit DColorComboBox(QWidget *parent = 0);
    void addData(const QColor &color, const QString &title);
    void addData(const QString &color, const QString &title);

signals:
    void currentColorChange(QColor color);

public slots:
    void onCurrentIndexChange(int index);

private:
    D_DECLARE_PRIVATE(DColorComboBox)
};

DWIDGET_END_NAMESPACE

#endif // DCOLORCOMBOBOX_H
