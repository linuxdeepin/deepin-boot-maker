// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

//#pragma once

//#include <dborderlesswindow.h>
//#include <QScopedPointer>

//#define BMWindowBaseClass DTK_WIDGET_NAMESPACE::DBorderlessWindow

//class BMWindowPrivate;
//class BMWindow: public DTK_WIDGET_NAMESPACE::DBorderlessWindow
//{
//    Q_OBJECT
//public:
//    BMWindow(QWidget *parent = nullptr);
//    ~BMWindow();
//private:
//    QScopedPointer<BMWindowPrivate> d_ptr;
//    Q_DECLARE_PRIVATE_D(qGetPtrHelper(d_ptr), BMWindow)
//};

#pragma once

#include <DMainWindow>

#include <QScopedPointer>

#define BMWindowBaseClass DTK_WIDGET_NAMESPACE::DMainWindow

class BMWindowPrivate;
class BMWindow: public DTK_WIDGET_NAMESPACE::DMainWindow
{
    Q_OBJECT
public:
    BMWindow(QWidget *parent = nullptr);
    ~BMWindow();
     virtual void closeEvent(QCloseEvent *event);
public slots:
    void slot_ThemeChange();
private:
    bool closeflags = true;
    QScopedPointer<BMWindowPrivate> d_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(d_ptr), BMWindow)
};

