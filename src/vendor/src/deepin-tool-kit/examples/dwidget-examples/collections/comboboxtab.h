/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef COMBOBOXTAB_H
#define COMBOBOXTAB_H

#include <QWidget>
#include <QLabel>
#include <QJsonObject>
#include <QAbstractItemView>
#include "dcombobox.h"
#include "dcomboboxmodel.h"
#include "dfontcombobox.h"
#include "dcolorcombobox.h"
#include "dsimplecombobox.h"

DWIDGET_USE_NAMESPACE

class ComboBoxTab : public QLabel
{
    Q_OBJECT
public:
    explicit ComboBoxTab(QWidget *parent = 0);

signals:

private:
    DComboBox *colorCB;
    DComboBoxModel * colorModel;
    DComboBox *cb;
    DComboBoxModel * model;
    DComboBox *simpleBox;
};

#endif // COMBOBOXTAB_H
