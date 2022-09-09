// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#ifndef UNMOUNTUSBVIEW_H
#define UNMOUNTUSBVIEW_H

#include <DWidget>
#include <QPointer>
#include <DLabel>
#include <DWaterProgress>
#include <DTipLabel>
#include <QVBoxLayout>
#include <DPalette>
#include <DApplicationHelper>
#include <QBrush>
#include <DFontSizeManager>
#include "widgetutil.h"
#include <DSpinner>
DWIDGET_USE_NAMESPACE
class UnmountUsbView : public DWidget
{
    Q_OBJECT
public:
    explicit UnmountUsbView(QWidget *parent = nullptr);
private:
    QPointer<DLabel>m_hitTitle;
    QPointer<DSpinner>m_spinner;
signals:
    void startSpinner();
    void finish1(quint32 error, const QString &title, const QString &description);
    void pauseSpinner();
public slots:
};

#endif // UNMOUNTUSBVIEW_H
