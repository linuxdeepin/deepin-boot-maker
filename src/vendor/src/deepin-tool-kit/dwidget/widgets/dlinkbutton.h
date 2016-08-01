/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DLINKBUTTON_H
#define DLINKBUTTON_H

#include <QPushButton>

#include "dwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DLinkButton : public QPushButton
{
    Q_OBJECT

public:
    DLinkButton(const QString & text = QString(), QWidget * parent = 0);
};

DWIDGET_END_NAMESPACE

#endif // DLINKBUTTON_H
