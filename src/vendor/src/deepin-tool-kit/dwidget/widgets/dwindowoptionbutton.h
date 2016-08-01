/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DWINDOWOPTIONBUTTON_H
#define DWINDOWOPTIONBUTTON_H

#include "dimagebutton.h"

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DWindowOptionButton : public DImageButton
{
    Q_OBJECT
public:
    DWindowOptionButton(QWidget * parent = 0);
};

DWIDGET_END_NAMESPACE

#endif // DWINDOWOPTIONBUTTON_H
