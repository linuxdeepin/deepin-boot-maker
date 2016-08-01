/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "darrowbutton.h"
#include "dthememanager.h"

DWIDGET_BEGIN_NAMESPACE

ArrowButtonIcon::ArrowButtonIcon(QWidget *parent) :
    QLabel(parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

void ArrowButtonIcon::setArrowDirection(int direction)
{
    m_direction = direction;
    style()->unpolish(this);
    style()->polish(this);// force a stylesheet recomputation
}

void ArrowButtonIcon::setButtonState(int state)
{
    m_buttonState = state;
    style()->unpolish(this);
    style()->polish(this);// force a stylesheet recomputation
}

int ArrowButtonIcon::arrowDirection() const
{
    return m_direction;
}

int ArrowButtonIcon::buttonState() const
{
    return m_buttonState;
}

DArrowButton::DArrowButton(QWidget *parent)
    : QLabel(parent)
{
    D_THEME_INIT_WIDGET(DArrowButton);

    setFixedSize(IMAGE_BUTTON_WIDTH, BUTTON_HEIGHT);

    m_normalLabel = new ArrowButtonIcon(this);
    m_normalLabel->setObjectName("ArrowButtonIconNormal");
    m_normalLabel->setFixedSize(size());
    m_normalLabel->move(0, 0);

    m_hoverLabel = new ArrowButtonIcon(this);
    m_hoverLabel->setObjectName("ArrowButtonIconHover");
    m_hoverLabel->setFixedSize(size());
    m_hoverLabel->move(0, 0);
    m_hoverLabel->setVisible(false);

    m_pressLabel = new ArrowButtonIcon(this);
    m_pressLabel->setObjectName("ArrowButtonIconPress");
    m_pressLabel->setFixedSize(size());
    m_pressLabel->move(0, 0);
    m_pressLabel->setVisible(false);

    setArrowDirection(DArrowButton::ArrowDown);
}

void DArrowButton::setArrowDirection(ArrowDirection direction)
{
    m_arrowDirection = direction;
    updateIconDirection(direction);
}

int DArrowButton::arrowDirection() const
{
    return m_arrowDirection;
}

void DArrowButton::setButtonState(ArrowButtonState state)
{
    m_buttonState = state;
    updateIconState(state);
}

int DArrowButton::buttonState() const
{
    return m_buttonState;
}

void DArrowButton::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    emit mousePress();
    m_normalLabel->setVisible(false);
    m_hoverLabel->setVisible(false);
    m_pressLabel->setVisible(true);

    setButtonState(DArrowButton::ArrowStatePress);
}

void DArrowButton::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    emit mouseRelease();
    m_normalLabel->setVisible(true);
    m_hoverLabel->setVisible(false);
    m_pressLabel->setVisible(false);

    setButtonState(DArrowButton::ArrowStateNormal);
}

void DArrowButton::enterEvent(QEvent *)
{
    emit mouseEnter();
}

void DArrowButton::leaveEvent(QEvent *)
{
    emit mouseLeave();
}

void DArrowButton::initButtonState()
{
//    QStateMachine * machine = new QStateMachine(this);
//    QState * hoverState = new QState(machine);
//    hoverState->assignProperty(this,"pos",QPoint(0,0));
//    QState * hideState = new QState(machine);
//    hideState->assignProperty(this,"pos",QPoint(0,height()));

//    machine->setInitialState(hoverState);

//    QPropertyAnimation *sa = new QPropertyAnimation(this, "pos");
//    sa->setDuration(200);
//    sa->setEasingCurve(QEasingCurve::InSine);
//    connect(sa,&QPropertyAnimation::finished,this,&Panel::hasShown);

//    QPropertyAnimation *ha = new QPropertyAnimation(this, "pos");
//    ha->setDuration(200);
//    ha->setEasingCurve(QEasingCurve::InSine);
//    connect(ha,&QPropertyAnimation::finished,this,&Panel::hasHidden);

//    QSignalTransition *ts1 = hoverState->addTransition(this,SIGNAL(startHide()), hideState);
//    ts1->addAnimation(ha);
//    connect(ts1,&QSignalTransition::triggered,[=](int value = 2){
//        m_HSManager->SetState(value);
//    });
//    QSignalTransition *ts2 = hideState->addTransition(this,SIGNAL(startShow()),hoverState);
//    ts2->addAnimation(sa);
//    connect(ts2,&QSignalTransition::triggered,[=](int value = 0){
//        m_HSManager->SetState(value);
//    });

//    machine->start();
}

void DArrowButton::updateIconDirection(ArrowDirection direction)
{
    m_normalLabel->setArrowDirection(direction);
    m_hoverLabel->setArrowDirection(direction);
    m_pressLabel->setArrowDirection(direction);
}

void DArrowButton::updateIconState(ArrowButtonState state)
{
    m_normalLabel->setButtonState(state);
    m_hoverLabel->setButtonState(state);
    m_pressLabel->setButtonState(state);
}

DWIDGET_END_NAMESPACE
