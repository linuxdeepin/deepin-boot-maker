/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dbuttonlist.h"
#include "dthememanager.h"
#include "dconstants.h"
#include <QListWidget>
#include <QButtonGroup>
#include <QPushButton>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QLabel>
#include <QPoint>
#include <QResizeEvent>
#include <QEvent>
#include <QDebug>


IconButton::IconButton(const QString &Icon, const QString &text, QWidget *parent):
    QPushButton(text, parent),
    m_icon(Icon),
    m_text(text)
{
    initIconLabel();
    initConnect();
}

void IconButton::initConnect(){
    connect(this, SIGNAL(toggled(bool)), m_iconLabel, SLOT(setVisible(bool)));
}

void IconButton::initIconLabel(){
    m_iconLabel = new QLabel(this);
    setIconLeftMargin(20);
    m_iconLabel->hide();
    QImage image(m_icon);
    m_iconLabel->setPixmap(QPixmap::fromImage(image));
    m_iconLabel->setFixedSize(image.size());
}

void IconButton::setIconLeftMargin(int leftMargin){
    m_iconLabel->move(leftMargin, y());
}

void IconButton::resizeEvent(QResizeEvent *event){
    int height = event->size().height();
    m_iconLabel->move(m_iconLabel->x(), (height - m_iconLabel->height())/ 2 );
    QPushButton::resizeEvent(event);
}

void IconButton::setIconLabel(const QString &icon){
    m_icon = icon;
    QImage image(m_icon);
    m_iconLabel->setPixmap(QPixmap::fromImage(image));
    m_iconLabel->setFixedSize(image.size());
}

void IconButton::hideIconLabel(){
    m_iconLabel->hide();
}

void IconButton::updateStyle(){
    style()->unpolish(this);
    style()->polish(this);
    update();
}

void IconButton::enterEvent(QEvent *event){
    emit mouseEntered(text());
    QPushButton::enterEvent(event);
}

void IconButton::leaveEvent(QEvent *event){
    emit mouseLeaved(text());
    QPushButton::leaveEvent(event);
}


DWIDGET_BEGIN_NAMESPACE

DButtonList::DButtonList(QWidget *parent) : QListWidget(parent)
{
    D_THEME_INIT_WIDGET(DButtonList);

    setSelectionMode(DButtonList::NoSelection);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollMode(ScrollPerItem);
    setResizeMode(Adjust);

    setItemSize(200, BUTTON_HEIGHT);

    m_buttonGroup = new QButtonGroup(this);
    m_buttonGroup->setExclusive(true);

    initConnect();
}


void DButtonList::initMargins(int leftMargin, int rightMargin, int imageLeftMargin){
    m_leftMargin = leftMargin;
    m_rightMargin = rightMargin;
    m_imageLeftMargin = imageLeftMargin;
}

DButtonList::~DButtonList()
{

}

void DButtonList::initConnect(){
    connect(m_buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(setButtonChecked(int)));
}

void DButtonList::setItemWidth(int width){
    setGridSize(QSize(width, gridSize().height()));
    setFixedWidth(gridSize().width());
}

void DButtonList::setItemHeight(int height){
    setGridSize(QSize(gridSize().width(), height));
}

void DButtonList::setItemSize(int width, int height){
    setItemSize(QSize(width, height));
}

void DButtonList::setItemSize(QSize size){
    setGridSize(size);
    setFixedWidth(gridSize().width());

    for(int i=0; i< count(); i++){
        itemWidget(item(i))->setFixedHeight(size.height());
    }
}

void DButtonList::addButton(const QString &label){
    int index = count();
    addButton(label, index);
}

void DButtonList::addButton(const QString &label, int index){
    IconButton* button = new IconButton(":/images/dark/images/tick_hover.png", label, this);
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    button->setCheckable(true);
    connect(button, SIGNAL(mouseEntered(QString)), this, SIGNAL(buttonMouseEntered(QString)));
    connect(button, SIGNAL(mouseLeaved(QString)), this, SIGNAL(buttonMouseLeaved(QString)));
    QFrame* borderFrame = new QFrame;
    borderFrame->setObjectName("BorderFrame");
    QVBoxLayout* borderLayout = new QVBoxLayout;
    borderLayout->addWidget(button);
    borderLayout->setSpacing(0);
    borderLayout->setContentsMargins(m_leftMargin, 0, m_rightMargin, 0);
    borderFrame->setLayout(borderLayout);

    m_buttonGroup->addButton(button, index);
    QListWidgetItem* item = new QListWidgetItem(this);
    addItem(item);
    setItemWidget(item, borderFrame);

    setItemSize(gridSize());

}


void DButtonList::addButtons(const QStringList &listLabels){
    for(int i= 0; i< listLabels.length(); i++) {
        addButton(listLabels.at(i), i);
    }
}

void DButtonList::setButtonChecked(int id){
   if (id < m_buttonGroup->buttons().length()){
       IconButton* button = reinterpret_cast<IconButton*>(m_buttonGroup->button(id));
       button->setChecked(true);
       if (m_buttonGroup->buttons().length() == 1){
            button->setProperty("state", "OnlyOne");
            button->hideIconLabel();
       }else{
            button->setProperty("state", "normal");
       }
       button->updateStyle();
       emit buttonCheckedIndexChanged(id);
       emit buttonChecked(button->text());
   }
}

void DButtonList::checkButtonByIndex(int index){
    if (index < m_buttonGroup->buttons().length()){
        IconButton* button = reinterpret_cast<IconButton*>(m_buttonGroup->button(index));
        button->click();
    }
}

IconButton* DButtonList::getButtonByIndex(int index){
    if (index < m_buttonGroup->buttons().length()){
        IconButton* button = reinterpret_cast<IconButton*>(m_buttonGroup->button(index));
        return button;
    }else{
        qWarning() << "There is no this index:" << index;
    }
    return NULL;
}

void DButtonList::clear(){
    foreach (QAbstractButton* button, m_buttonGroup->buttons()) {
        qDebug() << static_cast<IconButton*>(button)->text();
        static_cast<IconButton*>(button)->disconnect();
        m_buttonGroup->removeButton(static_cast<IconButton*>(button));
    }
    QListWidget::clear();
}


DWIDGET_END_NAMESPACE
