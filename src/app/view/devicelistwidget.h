// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <DListWidget>

DWIDGET_USE_NAMESPACE

class DeviceListWidget : public DListWidget
{
    Q_OBJECT
public:
    explicit DeviceListWidget(DWidget *parent = nullptr);
};
