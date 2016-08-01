/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef INPUTTAB_H
#define INPUTTAB_H

#include <QWidget>

#include "dpasswordedit.h"
#include "dsearchedit.h"
#include "dlineedit.h"

class InputTab : public QLabel
{
    Q_OBJECT
public:
    explicit InputTab(QWidget *parent = 0);
};

#endif // INPUTTAB_H
