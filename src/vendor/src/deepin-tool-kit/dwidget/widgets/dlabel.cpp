/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dlabel.h"
#include "dthememanager.h"

DWIDGET_USE_NAMESPACE

DLabel::DLabel(QWidget *parent, Qt::WindowFlags f)
    : QLabel(parent, f)
{
    D_THEME_INIT_WIDGET(DLabel);
}


DLabel::DLabel(const QString &text, QWidget *parent, Qt::WindowFlags f)
    : QLabel(text, parent, f)
{
    D_THEME_INIT_WIDGET(DLabel);
}

