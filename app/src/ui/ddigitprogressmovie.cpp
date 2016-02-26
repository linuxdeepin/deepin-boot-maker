/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "ddigitprogressmovie.h"

#include <QPainter>
#include <QBoxLayout>

DDigitProgressMovie::DDigitProgressMovie(QWidget *parent) :
    QWidget(parent)
{
    this->setStyleSheet("DDigitProgressAnimation { border: 1px solid gray; }");
}

void DDigitProgressMovie::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.save();
    QRectF target(10.0, 20.0, 80.0, 60.0);
    QRectF source(0.0, 0.0, 70.0, 40.0);
    QPixmap pixmap(":/ui/images/usb/1.png");
    p.drawPixmap(0, 0, pixmap);
    p.drawPixmap(target, pixmap, source);
    p.restore();
}
