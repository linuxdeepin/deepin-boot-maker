/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dslider.h"
#include "slidertab.h"

DWIDGET_USE_NAMESPACE

SliderTab::SliderTab(QWidget *parent) : QLabel(parent)
{
    setStyleSheet("SliderTab{background-color:  #1A1B1B}");

    DSlider * slider = new DSlider(Qt::Horizontal, this);
    slider->setFixedWidth(300);
    slider->setFixedHeight(40);
    slider->move(20, 50);

    DSlider *sharpSlider = new DSlider(Qt::Horizontal, this);
    sharpSlider->setHandleType(DSlider::SharpHandler);
    sharpSlider->setFixedWidth(300);
    sharpSlider->setFixedHeight(40);
    sharpSlider->move(20, 100);

}

