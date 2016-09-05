/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QTabWidget>
#include <QVBoxLayout>

#include "bartab.h"
#include "buttontab.h"
#include "inputtab.h"
#include "linetab.h"
#include "slidertab.h"
#include "indicatortab.h"
#include "widgetstab.h"
#include "comboboxtab.h"

#include <DMainWindow>

DWIDGET_USE_NAMESPACE

DWIDGET_BEGIN_NAMESPACE
class DAction;
DWIDGET_END_NAMESPACE

class MainWindow : public DMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void menuItemInvoked(DAction *action);

private:
    void initTabWidget();

private:
    QTabWidget *m_mainTab = NULL;
};

#endif // MAINWINDOW_H
