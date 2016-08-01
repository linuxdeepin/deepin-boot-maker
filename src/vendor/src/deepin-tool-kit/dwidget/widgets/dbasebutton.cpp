/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dbasebutton.h"
#include "dthememanager.h"

DWIDGET_USE_NAMESPACE

DBaseButton::DBaseButton(QWidget *parent) :
    QPushButton(parent)
{
    D_THEME_INIT_WIDGET(DBaseButton);

    initInsideFrame();
}

DBaseButton::DBaseButton(const QString &text, QWidget *parent) :
    QPushButton(text, parent)
{
    D_THEME_INIT_WIDGET(DBaseButton);

    initInsideFrame();
}

DBaseButton::DBaseButton(const QIcon &icon, const QString &text, QWidget *parent) :
    QPushButton(icon, text, parent)
{
    D_THEME_INIT_WIDGET(DBaseButton);

    initInsideFrame();
}

//Bypassing the problem here
//qss can't draw box-shadow
void DBaseButton::initInsideFrame()
{
    QFrame *insideFrame = new QFrame;
    insideFrame->raise();
    insideFrame->setAttribute(Qt::WA_TransparentForMouseEvents);
    insideFrame->setObjectName("ButtonInsideFrame");
    QHBoxLayout *insideLayout = new QHBoxLayout(this);
    insideLayout->setContentsMargins(0, 1, 0, 0);
    insideLayout->addWidget(insideFrame);
}
