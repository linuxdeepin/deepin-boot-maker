/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dthememanager.h"

#include "dwindowclosebutton.h"

DWIDGET_BEGIN_NAMESPACE

DWindowCloseButton::DWindowCloseButton(QWidget * parent) :
    DImageButton(parent)
{
    D_THEME_INIT_WIDGET(DWindowCloseButton);
}

DWIDGET_END_NAMESPACE
