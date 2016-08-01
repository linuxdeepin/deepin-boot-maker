/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DLISTVIEW_P_H
#define DLISTVIEW_P_H

#include "dlistview.h"

#include <private/dobject_p.h>

DWIDGET_BEGIN_NAMESPACE

class DBoxWidget;
class DListViewPrivate : public DObjectPrivate
{
    DListViewPrivate(DListView *qq);
    ~DListViewPrivate();

    void init();

    void onOrientationChanged();

    DBoxWidget *headerLayout = nullptr;
    DBoxWidget *footerLayout = nullptr;

    QList<QWidget*> headerList;
    QList<QWidget*> footerList;

#if(QT_VERSION < 0x050500)
    int left = 0, top = 0, right = 0, bottom = 0; // viewport margin
#endif

    D_DECLARE_PUBLIC(DListView)
};

DWIDGET_END_NAMESPACE

#endif // DLISTVIEW_P_H

