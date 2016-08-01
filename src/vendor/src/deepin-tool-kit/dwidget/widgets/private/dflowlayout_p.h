/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DFLOWLAYOUT_P_H
#define DFLOWLAYOUT_P_H

#include "dflowlayout.h"

#include <private/dobject_p.h>

class QLayoutItem;

DWIDGET_BEGIN_NAMESPACE

class DFlowLayoutPrivate : public DObjectPrivate
{
    DFlowLayoutPrivate(DFlowLayout *qq);

    QSize doLayout(const QRect &rect, bool testOnly) const;

    QList<QLayoutItem*> itemList;
    int horizontalSpacing = 0;
    int verticalSpacing = 0;
    mutable QSize sizeHint;
    DFlowLayout::Flow flow = DFlowLayout::Flow::LeftToRight;

    D_DECLARE_PUBLIC(DFlowLayout)
};

DWIDGET_END_NAMESPACE

#endif // DFLOWLAYOUT_P_H

