/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef WIDGETSTAB_H
#define WIDGETSTAB_H

#include <QLabel>

#include "dtextbutton.h"

DWIDGET_USE_NAMESPACE

class WidgetsTab : public QLabel
{
    Q_OBJECT
public:
    explicit WidgetsTab(QWidget *parent = 0);
};

#endif // WIDGETSTAB_H
