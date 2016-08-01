/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>

#include "dslider.h"
#include "dthememanager.h"
#include "dwidget_global.h"
#include "dswitchbutton.h"
#include "segmentedcontrol.h"
#include "dcolorpicker.h"

#include "mainwindow.h"
#include "buttonlisttab.h"
#include "buttongridtab.h"
#include "imagebuttongridtab.h"
#include "cameraform.h"
#include "graphicseffecttab.h"

DWIDGET_USE_NAMESPACE

MainWindow::MainWindow(DWindow *parent)
    : DWindow(parent)
{
    DThemeManager *themeManager = DThemeManager::instance();

    initTabWidget();

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->addWidget(m_mainTab);

    QHBoxLayout *styleLayout = new QHBoxLayout();
    QPushButton *darkButton = new QPushButton("Dark", this);
    QPushButton *lightBUtton = new QPushButton("Light", this);
    connect(darkButton, &QPushButton::clicked, [ = ] {
        themeManager->setTheme("dark");
    });
    connect(lightBUtton, &QPushButton::clicked, [ = ] {
        themeManager->setTheme("light");
    });
    styleLayout->addWidget(darkButton);
    styleLayout->addWidget(lightBUtton);
    styleLayout->addStretch();

    mainLayout->addLayout(styleLayout);

    this->setContentLayout(mainLayout);

    dbusMenu()->addAction("testmenu1");
    dbusMenu()->addAction("testmenu2");

#ifdef Q_OS_LINUX
    DMenu *menu = dbusMenu()->addMenu("menu1");

    menu->addAction("menu1->action1");
    menu->addAction("menu2->action2");

    connect(dbusMenu(), &DMenu::triggered, this, &MainWindow::menuItemInvoked);
#endif
}

void MainWindow::menuItemInvoked(DAction *action)
{
    QMessageBox::warning(this, "menu clieck",  action->text() + ", was cliecked");
    qDebug() << "click" << action << action->isChecked();
}

void MainWindow::initTabWidget()
{
    m_mainTab = new QTabWidget(this);

    ComboBoxTab *comboBoxTab = new ComboBoxTab(this);

    LineTab *lineTab = new LineTab(this);

    BarTab *barTab = new BarTab(this);

    ButtonTab *buttonTab = new ButtonTab(this);

    InputTab *inputTab = new InputTab(this);

    SliderTab *sliderTab = new SliderTab(this);

    IndicatorTab *indicatorTab = new IndicatorTab(this);

    ButtonListTab *buttonListGroupTab = new ButtonListTab(this);

    ButtonGridTab *buttonGridTab = new ButtonGridTab(this);

    ImageButtonGridTab *imageButtonGridTab = new ImageButtonGridTab(this);

    Segmentedcontrol *segmentedControl = new Segmentedcontrol(this);

    WidgetsTab *widgetsTab = new WidgetsTab(this);

    DColorPicker *picker = new DColorPicker(1, this);

    CameraForm *cameraform = new CameraForm(this);

    GraphicsEffectTab *effectTab = new GraphicsEffectTab;

    m_mainTab->addTab(effectTab, "GraphicsEffect");
    m_mainTab->addTab(comboBoxTab, "ComboBox");
    m_mainTab->addTab(indicatorTab, "Indicator");
    m_mainTab->addTab(lineTab, "Line");
    m_mainTab->addTab(barTab, "Bar");
    m_mainTab->addTab(buttonTab, "Button");
    m_mainTab->addTab(inputTab, "Input");
    m_mainTab->addTab(sliderTab, "Slider");

    m_mainTab->addTab(buttonListGroupTab, "ButtonList");
    m_mainTab->addTab(imageButtonGridTab, "imageButtonGrid");
    m_mainTab->addTab(buttonGridTab, "ButtonGrid");
    m_mainTab->addTab(segmentedControl, "Segmented Control");
    m_mainTab->addTab(widgetsTab, "Widgets");
    m_mainTab->addTab(picker, "Color Picker");
    m_mainTab->addTab(cameraform, "Camera View");

    m_mainTab->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{

}
