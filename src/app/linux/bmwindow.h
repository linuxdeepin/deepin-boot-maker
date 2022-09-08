// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <DMainWindow>
#include <DWidget>

#include <QScopedPointer>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>

DWIDGET_USE_NAMESPACE

#define BMWindowBaseClass DTK_WIDGET_NAMESPACE::DMainWindow

class BMWindowPrivate;
class BMWindow: public DTK_WIDGET_NAMESPACE::DMainWindow
{
    Q_OBJECT
public:
    BMWindow(QWidget *parent = nullptr);
    ~BMWindow();
public slots:
    void slot_ThemeChange();

private:
    //iDirection:-1 从右往左滑动; 1 从左往右滑动
    void slideWidget(DWidget *left, DWidget *right, int iDirection = -1);

private:
    QScopedPointer<BMWindowPrivate> d_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(d_ptr), BMWindow)
    QSequentialAnimationGroup* m_pSAnimationGroup;
};

class SlideAnimatoin:public QParallelAnimationGroup
{
    Q_OBJECT
public:
    SlideAnimatoin(QObject* pParent = nullptr);
    ~SlideAnimatoin();

public:
    //iDirection:-1 从右往左滑动; 1 从左往右滑动
    void initAnimation(DWidget* pLeftWidget, DWidget* pRightWidget, int iDirection = -1);

private:
    void setLeftWidget(DWidget* pWidget);
    void setRightWidget(DWidget* pWidget);

private slots:
    void slot_AnimationGroupFinished();

private:
    DWidget* m_pLeftWidget;
    DWidget* m_pRightWidget;
};
