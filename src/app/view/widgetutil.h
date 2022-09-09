// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <DWidget>

#include <QString>

DWIDGET_USE_NAMESPACE
namespace WidgetUtil {
QString getQss(const QString &className);
QPixmap getDpiPixmap(const QString filename, DWidget *w = Q_NULLPTR);
}

