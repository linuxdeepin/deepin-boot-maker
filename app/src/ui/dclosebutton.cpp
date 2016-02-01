/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dclosebutton.h"

DCloseButton::DCloseButton(QWidget *parent) :
    QPushButton(parent)
{
    QPixmap pixmap (":/ui/images/window_close_normal.png");
    this->setFixedSize(pixmap.size());

    QString style = "DCloseButton{"
        "background:url(:/ui/images/window_close_normal.png);"
        "border:0px;"
    "}"
    "DCloseButton:hover{"
        "background:url(:/ui/images/window_close_hover.png);"
        "border:0px;"
    "}"
    "DCloseButton:pressed{"
        "background:url(:/ui/images/window_close_press.png);"
        "border:0px;"
    "}";
    this->setStyleSheet(style);

    setFocusPolicy(Qt::NoFocus);
}
