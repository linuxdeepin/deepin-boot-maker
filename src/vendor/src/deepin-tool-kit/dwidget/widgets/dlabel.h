/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DLABEL_H
#define DLABEL_H


#include <QLabel>

#include "dwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DLabel : public QLabel
{
    Q_OBJECT

public:
    DLabel(QWidget * parent = 0, Qt::WindowFlags f = 0);
    DLabel(const QString & text, QWidget * parent = 0, Qt::WindowFlags f = 0);
};

DWIDGET_END_NAMESPACE

#endif // DLABEL_H
