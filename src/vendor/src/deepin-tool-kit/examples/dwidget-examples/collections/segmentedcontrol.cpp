/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "segmentedcontrol.h"
#include <QLabel>

Segmentedcontrol::Segmentedcontrol(QWidget *parent) :
    QFrame(parent),
    segmentedControl(new DSegmentedControl(this))
{
    setStyleSheet("Segmentedcontrol{background: #1e1e1e;}");

    segmentedControl->addSegmented("page1");
    segmentedControl->addSegmented("page2");
    segmentedControl->addSegmented("page3");
    segmentedControl->move(100, 50);

    QLabel *label = new QLabel(this);
    QPalette pa = label->palette();
    pa.setColor(QPalette::Foreground, Qt::white);
    label->setPalette(pa);
    label->move(100, 150);
    label->setText(QString("current paga %1").arg(segmentedControl->currentIndex()+1));

    connect(segmentedControl, &DSegmentedControl::currentChanged, [=](){
        label->setText(QString("current paga %1").arg(segmentedControl->currentIndex()+1));
    });
}

