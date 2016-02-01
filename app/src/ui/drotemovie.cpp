/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "drotemovie.h"
#include <QPixmap>
#include <QTransform>
#include <QTimer>
#include <QPainter>

DRoteMovie::DRoteMovie(const QPixmap &pixmap, QWidget *parent) :
    QWidget(parent)
{
    pixmap_ = pixmap;
    setFixedSize(pixmap_.size());

    timer_ = new QTimer(this);
    timer_->setInterval(25);
    connect(timer_, SIGNAL(timeout()), this, SLOT(refresh()));
}

void DRoteMovie::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);
    QPointF center(pixmap_.width() / qreal(2), pixmap_.height() / qreal(2));
    painter.translate(center);
    painter.rotate(degrees_);
    painter.translate(-center);
    painter.drawPixmap(QPointF(0, 0), pixmap_);
}

void DRoteMovie::refresh(){
    degrees_ += 5;
    update();
}

void DRoteMovie::start() {
    timer_->start();
}

void DRoteMovie::stop() {
    timer_->stop();
}

void DRoteMovie::setPixmap(const QPixmap &pixmap) {
    pixmap_ = pixmap;
    update();
}
