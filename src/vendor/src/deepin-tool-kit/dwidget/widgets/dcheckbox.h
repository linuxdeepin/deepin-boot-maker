/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DCHECKBOX_H
#define DCHECKBOX_H

#include <QCheckBox>

#include "dwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class DCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    explicit DCheckBox(QWidget *parent = 0);
    explicit DCheckBox(const QString &text, QWidget *parent=0);
};

DWIDGET_END_NAMESPACE

#endif // DCHECKBOX_H
