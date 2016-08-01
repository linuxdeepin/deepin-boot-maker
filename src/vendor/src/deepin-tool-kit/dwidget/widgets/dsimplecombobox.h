/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DSIMPLECOMBOBOX_H
#define DSIMPLECOMBOBOX_H

#include <QWidget>

#include "dwidget_global.h"
#include "dcomboboxmodel.h"
#include "dabstractcomboboxdelegate.h"
#include "dcombobox.h"

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DSimpleComboBox : public DComboBox
{
    Q_OBJECT
public:
    explicit DSimpleComboBox(QWidget *parent = 0);

    void addItem(const QString &text);
    void addItems(const QStringList &texts);

signals:
    void currentTextChanged(const QString &);

private:
    Q_SLOT void onCurrentIndexChange(int index);
};

DWIDGET_END_NAMESPACE

#endif // DSIMPLECOMBOBOX_H
