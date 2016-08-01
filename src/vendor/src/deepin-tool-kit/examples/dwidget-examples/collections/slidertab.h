/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef SLIDERTAB_H
#define SLIDERTAB_H

#include <QWidget>
#include <QLabel>

class SliderTab : public QLabel
{
    Q_OBJECT
public:
    explicit SliderTab(QWidget *parent = 0);
};

#endif // SLIDERTAB_H
