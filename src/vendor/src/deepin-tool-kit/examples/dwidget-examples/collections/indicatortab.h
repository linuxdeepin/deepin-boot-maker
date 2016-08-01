/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef INDICATORTAB_H
#define INDICATORTAB_H

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "dloadingindicator.h"

DWIDGET_USE_NAMESPACE

class IndicatorTab : public QLabel
{
    Q_OBJECT
public:
    explicit IndicatorTab(QWidget *parent = 0);
};

#endif // INDICATORTAB_H
