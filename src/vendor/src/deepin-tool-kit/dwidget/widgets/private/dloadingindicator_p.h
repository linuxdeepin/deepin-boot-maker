/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DLOADINGINDICATOR_P
#define DLOADINGINDICATOR_P

#include "dloadingindicator.h"

#include <private/dobject_p.h>

DWIDGET_BEGIN_NAMESPACE

class DLoadingIndicatorPrivate : public DObjectPrivate
{
    DLoadingIndicatorPrivate(DLoadingIndicator *qq);

    void init();
    void setLoadingItem(QGraphicsItem *item);

    QVariantAnimation rotateAni;
    bool loading;
    QWidget* widgetSource = NULL;
    bool smooth = false;
    DLoadingIndicator::RotationDirection direction = DLoadingIndicator::Clockwise;

    D_DECLARE_PUBLIC(DLoadingIndicator)
};

DWIDGET_END_NAMESPACE

#endif // DLOADINGINDICATOR_P

