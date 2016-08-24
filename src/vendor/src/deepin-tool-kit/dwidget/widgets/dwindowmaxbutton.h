/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DWINDOWMAXBUTTON_H
#define DWINDOWMAXBUTTON_H

#include <dobject.h>

#include "dimagebutton.h"

DWIDGET_BEGIN_NAMESPACE

class DWindowMaxButtonPrivate;

class LIBDTKWIDGETSHARED_EXPORT DWindowMaxButton : public DImageButton, public DObject
{
    Q_OBJECT
public:
    DWindowMaxButton(QWidget * parent = 0);

    Q_PROPERTY(bool isMaximized READ isMaximized WRITE setMaximized NOTIFY maximizedChanged)

    bool isMaximized() const;

    Q_DECL_DEPRECATED void setWindowState(Qt::WindowState windowState);

public slots:
    void setMaximized(bool isMaximized);

signals:
    Q_DECL_DEPRECATED void maximum();
    Q_DECL_DEPRECATED void restore();

    void maximizedChanged(bool isMaximized);

private:
    D_DECLARE_PRIVATE(DWindowMaxButton)
};

DWIDGET_END_NAMESPACE

#endif // DWINDOWMAXBUTTON_H
