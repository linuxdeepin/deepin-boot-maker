/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "graphicseffecttab.h"

#include <DBlurEffectWidget>

#include <QLabel>
#include <QDebug>
DWIDGET_USE_NAMESPACE

GraphicsEffectTab::GraphicsEffectTab(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    DBlurEffectWidget *blurWidget = new DBlurEffectWidget(this);
    blurWidget->setRadius(30);
    blurWidget->setFixedSize(200, 200);

    QVBoxLayout *blurLayout = new QVBoxLayout(blurWidget);

    QPushButton *glowEffectButton = new QPushButton("GlowEffect");
    glowEffectButton->setFixedWidth(100);

    DGraphicsGlowEffect *effect = new DGraphicsGlowEffect();
    effect->setBlurRadius(5.0);
    effect->setDistance(15);
    effect->setYOffset(3);
    effect->setColor(Qt::darkCyan);
    glowEffectButton->setGraphicsEffect(effect);

    blurLayout->addWidget(glowEffectButton, 0, Qt::AlignCenter);

    mainLayout->addWidget(blurWidget, 0, Qt::AlignCenter);
}

GraphicsEffectTab::~GraphicsEffectTab()
{

}

void GraphicsEffectTab::paintEvent(QPaintEvent *pe)
{
    QPainter paint(this);
    QPixmap bkgnd(":/images/wallpaper/time 14.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    paint.drawPixmap(0, 0, bkgnd);
    QWidget::paintEvent(pe);
}

