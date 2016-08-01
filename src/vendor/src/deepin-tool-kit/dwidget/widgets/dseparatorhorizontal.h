/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DSEPARATORHORIZONTAL_H
#define DSEPARATORHORIZONTAL_H

#include <QWidget>
#include <QVBoxLayout>

#include "dwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DSeparatorHorizontal : public QWidget
{
    Q_OBJECT
public:
    explicit DSeparatorHorizontal(QWidget *parent = 0);

private:
    QWidget *m_topRec = NULL;
    QWidget *m_bottomRec = NULL;
};

DWIDGET_END_NAMESPACE

#endif // DSEPARATORHORIZONTAL_H
