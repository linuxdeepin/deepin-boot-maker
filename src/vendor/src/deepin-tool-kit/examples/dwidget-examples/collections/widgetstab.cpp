/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "widgetstab.h"
#include "darrowrectangle.h"
#ifdef Q_OS_UNIX
#include "dcalendar.h"
#endif

#include <QCheckBox>

WidgetsTab::WidgetsTab(QWidget *parent) : QLabel(parent)
{
    setStyleSheet("WidgetsTab {background-color:#252627;} QCheckBox {color:#666;}");

#ifdef CALENDARWIDGET_H
    DTK_WIDGET_NAMESPACE::DCalendar *calendar = new DTK_WIDGET_NAMESPACE::DCalendar(this);
    calendar->setFixedSize(300, 300);
    calendar->move(8, 8);
//    calendar->setLunarFestivalHighlight(false);
#endif

    QCheckBox *lunarVisible = new QCheckBox(this);
    lunarVisible->setText("lunar visible");
    lunarVisible->move(320, 50);
    lunarVisible->setChecked(true);
    QCheckBox *festivalHighlight = new QCheckBox(this);
    festivalHighlight->setText("lunar festival highlight");
    festivalHighlight->setChecked(true);
    festivalHighlight->move(320, 80);
    QCheckBox *cellSelectable = new QCheckBox(this);
    cellSelectable->setText("cell selectable");
    cellSelectable->setChecked(true);
    cellSelectable->move(320, 110);
    QCheckBox *controlPanelVisible = new QCheckBox(this);
    controlPanelVisible->setText("control panel visible");
    controlPanelVisible->move(320, 140);
    controlPanelVisible->setChecked(true);
    QCheckBox *dateInfoVisible = new QCheckBox(this);
    dateInfoVisible->setText("date info visible");
    dateInfoVisible->setChecked(false);
    dateInfoVisible->move(320, 170);

#ifdef CALENDARWIDGET_H
    connect(cellSelectable, &QCheckBox::stateChanged, [calendar] (int state) -> void {calendar->setSelectionMode(DCalendar::SelectionMode(state));});
    connect(festivalHighlight, &QCheckBox::stateChanged, [calendar] (int state) -> void {calendar->setLunarFestivalHighlight(state);});
    connect(lunarVisible, &QCheckBox::stateChanged, [calendar] (int state) -> void {calendar->setLunarVisible(state);});
    connect(controlPanelVisible, &QCheckBox::stateChanged, [calendar] (int state) -> void {calendar->setControlPanelVisible(state);});
    connect(dateInfoVisible, &QCheckBox::stateChanged, [calendar] (int state) -> void {calendar->setDateInfoVisible(state);});
#endif

    ////////////////////ArrowRectangle//////////////////
    DArrowRectangle *rectangle = new DArrowRectangle(DArrowRectangle::ArrowLeft);
//    rectangle->setArrowY(15);
    rectangle->setArrowX(25);
    rectangle->setArrowWidth(30);
    rectangle->setArrowHeight(10);
    QPushButton *button = new QPushButton("DArrowRectangle Test");
    button->setFixedSize(200, 30);
    rectangle->setContent(button);
    rectangle->show(200, 200);
}

