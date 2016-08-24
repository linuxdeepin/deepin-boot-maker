/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <private/dobject_p.h>

#include "dthememanager.h"
#include "dwindowmaxbutton.h"

#include <QStyle>

DWIDGET_BEGIN_NAMESPACE

class DWindowMaxButtonPrivate : public DObjectPrivate{
public:
    DWindowMaxButtonPrivate(DWindowMaxButton* qq):DObjectPrivate(qq) {
        m_isMaximized = false;
    }

private:
    bool m_isMaximized;
    Q_DECLARE_PUBLIC(DWindowMaxButton)
};

DWindowMaxButton::DWindowMaxButton(QWidget * parent) :
    DImageButton(parent),
    DObject(*new DWindowMaxButtonPrivate(this))
{
    D_THEME_INIT_WIDGET(DWindowMaxButton, isMaximized);
}

bool DWindowMaxButton::isMaximized() const
{
    D_DC(DWindowMaxButton);

    return d->m_isMaximized;
}

void DWindowMaxButton::setWindowState(Qt::WindowState windowState)
{
    setMaximized(windowState == Qt::WindowMaximized);
}

void DWindowMaxButton::setMaximized(bool isMaximized)
{
    D_D(DWindowMaxButton);

    if (d->m_isMaximized == isMaximized)
        return;

    d->m_isMaximized = isMaximized;
    emit maximizedChanged(isMaximized);
}

DWIDGET_END_NAMESPACE


