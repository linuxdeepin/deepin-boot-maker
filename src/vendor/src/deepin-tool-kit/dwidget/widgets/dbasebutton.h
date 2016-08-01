/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DBASEBUTTON_H
#define DBASEBUTTON_H

#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>
#include "dwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DBaseButton : public QPushButton
{
    Q_OBJECT
public:
    explicit DBaseButton(QWidget * parent = 0);
    explicit DBaseButton(const QString & text, QWidget * parent = 0);
    explicit DBaseButton(const QIcon & icon, const QString & text, QWidget * parent = 0);

private:
    void initInsideFrame();
};

DWIDGET_END_NAMESPACE

#endif // DBASEBUTTON_H
