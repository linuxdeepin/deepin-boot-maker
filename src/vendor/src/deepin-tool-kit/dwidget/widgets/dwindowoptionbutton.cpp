/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dthememanager.h"

#include "dwindowoptionbutton.h"

DWIDGET_BEGIN_NAMESPACE

DWindowOptionButton::DWindowOptionButton(QWidget * parent) :
    DImageButton(parent)
{
    D_THEME_INIT_WIDGET(DWindowOptionButton);

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

DWIDGET_END_NAMESPACE


