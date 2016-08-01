/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dsegmentedcontrol.h"
#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include <QApplication>
#include "dthememanager.h"

DWIDGET_BEGIN_NAMESPACE

DSegmentedHighlight::DSegmentedHighlight(QWidget *parent) :
    QToolButton(parent)
{
}

DSegmentedControl::DSegmentedControl(QWidget *parent) :
    QFrame(parent),
    m_highlight(new DSegmentedHighlight(this)),
    m_hLayout(new QHBoxLayout(this)),
    m_highlightMoveAnimation(new QPropertyAnimation(m_highlight, "geometry", this)),
    m_currentIndex(-1)
{
    setObjectName("DSegmentedControl");
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_hLayout->setSpacing(0);
    m_hLayout->setMargin(0);
    m_hLayout->setObjectName("TabBar");
    m_highlight->setObjectName("Highlight");
    m_highlight->installEventFilter(this);

    D_THEME_INIT_WIDGET(DSegmentedControl);

    m_highlightMoveAnimation->setDuration(100);
    m_highlightMoveAnimation->setEasingCurve(QEasingCurve::InCubic);
}

int DSegmentedControl::count() const
{
    return m_tabList.count();
}

const DSegmentedHighlight *DSegmentedControl::highlight() const
{
    return m_highlight;
}

int DSegmentedControl::currentIndex() const
{
    return m_currentIndex;
}

QToolButton *DSegmentedControl::at(int index) const
{
    return m_tabList[index];
}

QString DSegmentedControl::getText(int index) const
{
    const QToolButton *button = at(index);

    if(button){
        return button->text();
    }

    return "";
}

QIcon DSegmentedControl::getIcon(int index) const
{
    return at(index)->icon();
}

int DSegmentedControl::animationDuration() const
{
    return m_highlightMoveAnimation->duration();
}

int DSegmentedControl::indexByTitle(const QString &title) const
{
    int i=0;
    foreach (QToolButton *button, m_tabList) {
        if(button->text() == title)
            return i;
        ++i;
    }

    return -1;
}

QEasingCurve::Type DSegmentedControl::animationType() const
{
    return m_highlightMoveAnimation->easingCurve().type();
}

int DSegmentedControl::addSegmented(const QString &title)
{
    insertSegmented(m_hLayout->count(), title);

    return m_hLayout->count()-1;
}

int DSegmentedControl::addSegmented(const QIcon &icon, const QString &title)
{
    insertSegmented(m_hLayout->count(), icon, title);

    return m_hLayout->count()-1;
}

void DSegmentedControl::addSegmented(const QStringList &titleList)
{
    foreach (const QString &title, titleList) {
        addSegmented(title);
    }
}

void DSegmentedControl::addSegmented(const QList<QIcon> &iconList, const QStringList &titleList)
{
    for(int i=0;i<titleList.count();++i){
        addSegmented(iconList[i], titleList[i]);
    }
}

void DSegmentedControl::insertSegmented(int index, const QString &title)
{
    insertSegmented(index, QIcon(), title);
}

void DSegmentedControl::insertSegmented(int index, const QIcon &icon, const QString &title)
{
    QToolButton *button = new QToolButton();

    m_tabList.insert(index, button);

    button->setObjectName("Segmented");
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    button->setText(title);
    button->setIcon(icon);

    connect(button, &QToolButton::clicked, this, &DSegmentedControl::buttonClicked);
    m_hLayout->insertWidget(index, button);

    if(m_currentIndex == -1){
        setCurrentIndex(0);
    }

    button->installEventFilter(this);
}

void DSegmentedControl::removeSegmented(int index)
{
    if(index == m_currentIndex)
        setCurrentIndex(-1);

    delete m_hLayout->takeAt(index);

    QToolButton *button = at(index);
    m_tabList.removeAt(index);
    if(button)
        button->deleteLater();
}

void DSegmentedControl::clear()
{
    for(int i=0; i<count(); ++i){
        delete m_hLayout->takeAt(i);

        QToolButton *button = at(i);
        if(button)
            button->deleteLater();
    }

    m_tabList.clear();
}

bool DSegmentedControl::setCurrentIndex(int currentIndex)
{
    if(currentIndex == m_currentIndex)
        return true;

    if(currentIndex<0||currentIndex>count()-1){
        qErrnoWarning("index range over!");
        return false;
    }

    m_currentIndex = currentIndex;

    foreach (QToolButton *button, m_tabList) {
        button->setEnabled(true);
    }

    at(currentIndex)->setFocus();
    at(currentIndex)->setEnabled(false);

    updateHighlightGeometry();

    emit currentChanged(currentIndex);
    emit currentTitleChanged(at(currentIndex)->text());

    return true;
}

bool DSegmentedControl::setCurrentIndexByTitle(const QString &title)
{
    return setCurrentIndex(indexByTitle(title));
}

void DSegmentedControl::setText(int index, const QString &title)
{
    at(index)->setText(title);
}

void DSegmentedControl::setIcon(int index, const QIcon &icon)
{
    at(index)->setIcon(icon);
}

void DSegmentedControl::setAnimationDuration(int animationDuration)
{
    m_highlightMoveAnimation->setDuration(animationDuration);
}

void DSegmentedControl::setAnimationType(QEasingCurve::Type animationType)
{
    m_highlightMoveAnimation->setEasingCurve(animationType);
}

bool DSegmentedControl::eventFilter(QObject *obj, QEvent *e)
{
    if(m_currentIndex < 0)
        return false;

    QWidget *w = at(m_currentIndex);

    if(obj == m_highlight){
        if(e->type() == QEvent::Move){
            updateHighlightGeometry(false);
        }
    }else if(obj == w){
        if(e->type() == QEvent::Resize){
            updateHighlightGeometry(false);
        }
    }

    return false;
}

void DSegmentedControl::updateHighlightGeometry(bool animation)
{
    if(m_currentIndex<0)
        return;

    QRect tmp = at(m_currentIndex)->geometry();

    if(m_currentIndex==0){
        tmp.setX(0);
        tmp.setWidth(tmp.width()+1);
    }else if(m_currentIndex == count()-1){
        tmp.setWidth(tmp.width()+1);
    }
    tmp.setY(0);

    if(m_highlight->geometry() == tmp)
        return;

    if(animation){
        m_highlightMoveAnimation->setStartValue(m_highlight->geometry());
        m_highlightMoveAnimation->setEndValue(tmp);
        m_highlightMoveAnimation->start();
    }else{
        m_highlight->setFixedSize(tmp.size());
        m_highlight->move(tmp.topLeft());
    }
}

void DSegmentedControl::buttonClicked()
{
    int i = m_tabList.indexOf(qobject_cast<QToolButton*>(sender()));

    if(i>=0){
        setCurrentIndex(i);
    }
}

DWIDGET_END_NAMESPACE
