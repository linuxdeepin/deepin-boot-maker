/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "graphicseffecttab.h"

GraphicsEffectTab::GraphicsEffectTab(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    QPushButton *glowEffectButton = new QPushButton("GlowEffect");
    glowEffectButton->setFixedWidth(100);
    DGraphicsGlowEffect *effect = new DGraphicsGlowEffect(this);
    effect->setBlurRadius(20.0);
    effect->setDistance(1);
    effect->setYOffset(3);
    glowEffectButton->setGraphicsEffect(effect);

    mainLayout->addWidget(glowEffectButton);
}

GraphicsEffectTab::~GraphicsEffectTab()
{

}

