/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef SEGMENTEDCONTROL_H
#define SEGMENTEDCONTROL_H

#include <QFrame>
#include "dsegmentedcontrol.h"
#include "dwidget_global.h"

DWIDGET_USE_NAMESPACE

class Segmentedcontrol : public QFrame
{
    Q_OBJECT
public:
    explicit Segmentedcontrol(QWidget *parent = 0);

private:
    DSegmentedControl *segmentedControl;
};

#endif // SEGMENTEDCONTROL_H
