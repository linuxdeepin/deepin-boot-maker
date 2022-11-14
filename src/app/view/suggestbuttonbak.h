// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

//#include <dbasebutton.h>
#include <DWidget>
#include <DPushButton>
#include <DGuiApplicationHelper>
#include <QMouseEvent>

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

class SuggestButton : public DPushButton
{
    Q_OBJECT
public:
    explicit SuggestButton(DWidget *parent = 0);
    void mSetText(QString text);
protected:
    void paintEvent(QPaintEvent *event) override;
    void  enterEvent(QEvent *e) override;
    void  leaveEvent(QEvent *e) override;
    void  mouseEvent(QMouseEvent *e);

private:
    qreal m_filletradii;
    bool  israised;
};

