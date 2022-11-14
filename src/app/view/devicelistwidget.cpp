// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "devicelistwidget.h"
#include "widgetutil.h"

DeviceListWidget::DeviceListWidget(DWidget *parent) : DListWidget(parent)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFocusPolicy(Qt::NoFocus);
    setFlow(QListView::TopToBottom);
    setMovement(QListView::Static);
    setViewMode(QListView::ListMode);
    setFrameShape(QListWidget::NoFrame);
}
