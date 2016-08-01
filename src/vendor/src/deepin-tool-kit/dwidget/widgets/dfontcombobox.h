/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DFONTCOMBOBOX_H
#define DFONTCOMBOBOX_H

#include <QLabel>
#include <QObject>
#include <QPainter>
#include <QPaintEvent>
#include <QJsonObject>
#include <QItemDelegate>
#include <QModelIndex>
#include <QCursor>
#include <QColor>

#include "dwidget_global.h"
#include "dcombobox.h"
#include "dcomboboxmodel.h"
#include "dabstractcomboboxdelegate.h"

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DFontComboBox : public DComboBox
{
    Q_OBJECT
public:
    explicit DFontComboBox(QWidget *parent = 0);
    void addFontItem(const QString &family, const QString &title = "");

signals:
    void currentFontNameChange(QString name);

private slots:
    void onCurrentIndexChange(int index);
};

DWIDGET_END_NAMESPACE

#endif // DFONTCOMBOBOX_H
