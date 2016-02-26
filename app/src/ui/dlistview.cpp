/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dlistview.h"

DListView::DListView(QWidget *parent) :
    QListView(parent)
{
    QString qss = "QListView {"
        "font-size: 12px;"
        "color:#b4b4b4;"
        "margin:0px;"
        "border-top: 1px solid rgba(0, 0, 0, 255);"
        "border-left: 1px solid rgba(0, 0, 0, 255);"
        "border-right: 1px solid rgba(0, 0, 0, 255);"
        "border-bottom: 2px solid qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,"
            "stop:0 rgba(0, 0, 0, 255),"
            "stop:0.66 rgba(0, 0, 0, 255),"
            "stop:0.67 rgb(50, 50, 50),"
            "stop:1 rgb(42, 42, 42));"
        "border-radius: 4px;"
        "background: #1a1b1b;"
    "}"
    "QListView::item {"
        "height: 24px;"
        "margin: 1px;"
        "border-bottom: 2px solid qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,"
        "stop:0 rgba(0, 0, 0, 255),"
        "stop:0.5 rgba(0, 0, 0, 255),"
        "stop:0.67 rgb(50, 50, 50),"
        "stop:1 rgb(50, 50, 50));"
    "}"
    "QListView::item:selected {"
        "color: #00bdfd;"
    "}"
    "QListView::item:hover {"
        "color: #ebab4c;"
    "}";
    this->setStyleSheet(qss);

    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setFocusPolicy(Qt::NoFocus);
}
