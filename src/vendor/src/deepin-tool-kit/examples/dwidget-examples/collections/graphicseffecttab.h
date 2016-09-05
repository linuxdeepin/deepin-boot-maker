/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef GRAPHICSEFFECTTAB_H
#define GRAPHICSEFFECTTAB_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

#include "dgraphicsgloweffect.h"

DWIDGET_USE_NAMESPACE

class GraphicsEffectTab : public QWidget
{
    Q_OBJECT
public:
    explicit GraphicsEffectTab(QWidget *parent = 0);
    ~GraphicsEffectTab();


    virtual void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

};

#endif // GRAPHICSEFFECTTAB_H
