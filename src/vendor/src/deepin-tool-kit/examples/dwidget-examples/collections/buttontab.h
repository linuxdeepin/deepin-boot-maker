/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef BUTTONTAB_H
#define BUTTONTAB_H

#include <QWidget>
#include <QLabel>

#include "dwidget_global.h"
#include "dtextbutton.h"
#include "dimagebutton.h"
#include "dheaderline.h"
#include "darrowbutton.h"
#include "dswitchbutton.h"
#include "dwindowmaxbutton.h"
#include "dwindowminbutton.h"
#include "dwindowrestorebutton.h"
#include "dwindowclosebutton.h"
#include "dwindowoptionbutton.h"
#include "dlinkbutton.h"
#include "dcalendar.h"

class ButtonTab : public QLabel
{
    Q_OBJECT
public:
    explicit ButtonTab(QWidget *parent = 0);

public slots:
    void buttonClickTest();
};

#endif // BUTTONTAB_H
