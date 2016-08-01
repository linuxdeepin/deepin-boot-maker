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
    D_THEME_INIT_WIDGET(DWindowMaxButton);
    connect(this, &DWindowMaxButton::clicked, this, &DWindowMaxButton::tirgger);
}

bool DWindowMaxButton::isMaximized() const
{
    D_DC(DWindowMaxButton);
    return d->m_isMaximized;
}

void DWindowMaxButton::tirgger() {
    D_D(DWindowMaxButton);
    if (d->m_isMaximized) {
        emit restore();
    } else {
        emit maximum();
    }

    d->m_isMaximized = !d->m_isMaximized;
    style()->unpolish(this);
    style()->polish(this);// force a stylesheet recomputation
}

void DWindowMaxButton::setWindowState(Qt::WindowState windowState)
{
        D_D(DWindowMaxButton);
     d->m_isMaximized = windowState == Qt::WindowMaximized;
     style()->unpolish(this);
     style()->polish(this);// force a stylesheet recomputation
}

DWIDGET_END_NAMESPACE


