/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "buttongridtab.h"
#include "dbuttongrid.h"

#include <QHBoxLayout>
#include <QDebug>

DWIDGET_USE_NAMESPACE

ButtonGridTab::ButtonGridTab(QWidget *parent) : QFrame(parent)
{
    initData();
    initUI();
    initConnect();
    setStyleSheet("background-color: rgba(52, 52, 52, 0.9)");
}

ButtonGridTab::~ButtonGridTab()
{

}

void ButtonGridTab::initData(){
    m_buttons << "Button1" << "Button2" << "Button3";
    m_buttons << "1ms" << "2ms" << "3ms";
    m_buttons << "aa" << "bb" << "cc" << "asassa";
}

void ButtonGridTab::initUI(){


    DButtonGrid* buttonGridGroup = new DButtonGrid(3, 4, this);
    buttonGridGroup->setItemSize(60, 40);
    buttonGridGroup->addButtons(m_buttons);
    buttonGridGroup->addButtons(m_buttons);
    buttonGridGroup->setShowGrid(false);
    connect(buttonGridGroup, SIGNAL(buttonChecked(QString)), this, SLOT(handleClicked(QString)));
    connect(buttonGridGroup, SIGNAL(buttonMouseEntered(QString)), this, SLOT(handleEnterEvent(QString)));
    connect(buttonGridGroup, SIGNAL(buttonMouseLeaved(QString)), this, SLOT(handleLeaveEvent(QString)));

    connect(buttonGridGroup, SIGNAL(buttonEnteredIndexChanged(int)), this, SLOT(handleEnterIndex(int)));
    connect(buttonGridGroup, SIGNAL(buttonLeavedIndexChanged(int)), this, SLOT(handleLeaveIndex(int)));

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(buttonGridGroup);
    layout->setContentsMargins(50, 50, 50, 50);
    setLayout(layout);

    buttonGridGroup->checkButtonByIndex(2);
    buttonGridGroup->addButtons(m_buttons);

    buttonGridGroup->clear();
    buttonGridGroup->addButtons(m_buttons);
    buttonGridGroup->checkButtonByIndex(2);
}


void ButtonGridTab::handleClicked(QString label){
    qDebug() << label << "clicked";
}

void ButtonGridTab::initConnect(){

}
void ButtonGridTab::handleEnterEvent(QString label) {
    qDebug() << "enterEvent" << label;
}
void ButtonGridTab::handleLeaveEvent(QString label) {
     qDebug() << "leaveEvent" << label;
}

void ButtonGridTab::handleEnterIndex(int index) {
    qDebug() << "Enter index:" << index;
}

void ButtonGridTab::handleLeaveIndex(int index) {
    qDebug() << "Leave index:" << index;
}
