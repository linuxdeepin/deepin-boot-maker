/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dimagebutton.h"
#include "dconstants.h"
#include "dthememanager.h"

#include <QMouseEvent>
#include <QEvent>

DWIDGET_BEGIN_NAMESPACE

DImageButton::DImageButton(QWidget *parent)
    : QLabel(parent)
{
    D_THEME_INIT_WIDGET(DImageButton);
    changeState();
}

DImageButton::DImageButton(const QString &normalPic, const QString &hoverPic, const QString &pressPic, QWidget *parent)
    : QLabel(parent)
{
    D_THEME_INIT_WIDGET(DImageButton);

    if (!normalPic.isEmpty())
        m_normalPic = normalPic;
    if (!hoverPic.isEmpty())
        m_hoverPic = hoverPic;
    if (!pressPic.isEmpty())
        m_pressPic = pressPic;

    setCheckable(false);

    changeState();
}

DImageButton::DImageButton(const QString &normalPic, const QString &hoverPic,
                           const QString &pressPic, const QString &checkedPic, QWidget *parent)
    : QLabel(parent)
{
    D_THEME_INIT_WIDGET(DImageButton);

    if (!normalPic.isEmpty())
        m_normalPic = normalPic;
    if (!hoverPic.isEmpty())
        m_hoverPic = hoverPic;
    if (!pressPic.isEmpty())
        m_pressPic = pressPic;
    if (!checkedPic.isEmpty())
        m_checkedPic = checkedPic;

    setCheckable(true);

    changeState();
}

DImageButton::~DImageButton()
{
}

void DImageButton::enterEvent(QEvent *event)
{
    setCursor(Qt::PointingHandCursor);

    if (!m_isChecked){
        m_state = Hover;
        changeState();
    }

    event->accept();
    //QLabel::enterEvent(event);
}

void DImageButton::leaveEvent(QEvent *event)
{
    if (!m_isChecked){
        m_state = Normal;
        changeState();
    }

    event->accept();
    //QLabel::leaveEvent(event);
}

void DImageButton::mousePressEvent(QMouseEvent *event)
{
    m_state = Press;
    changeState();

    event->accept();
    //QLabel::mousePressEvent(event);
}

void DImageButton::mouseReleaseEvent(QMouseEvent *event)
{
    m_state = Hover;
    changeState();

    emit clicked();

    if (m_isCheckable){
        m_isChecked = !m_isChecked;
        if (m_isChecked){
            m_state = Checked;
        } else {
            m_state = Normal;
        }
        changeState();
    }

    event->accept();
    //QLabel::mouseReleaseEvent(event);
}

void DImageButton::changeState()
{
    switch (m_state) {
    case Hover:     if (!m_hoverPic.isEmpty()) setPixmap(QPixmap(m_hoverPic));      break;
    case Press:     if (!m_pressPic.isEmpty()) setPixmap(QPixmap(m_pressPic));      break;
    case Checked:   if (!m_checkedPic.isEmpty()) setPixmap(QPixmap(m_checkedPic));  break;
    default:        if (!m_normalPic.isEmpty()) setPixmap(QPixmap(m_normalPic));    break;
    }

    emit stateChanged();
}

void DImageButton::setCheckable(bool flag)
{
    m_isCheckable = flag;

    if (!m_isCheckable){
        m_state = Normal;
        changeState();
    }
}

void DImageButton::setChecked(bool flag)
{
    if (m_isCheckable == false){
        return;
    }

    m_isChecked = flag;
    if (m_isChecked){
        m_state = Checked;        
    } else {
        m_state = Normal;
    }
    changeState();
}

bool DImageButton::isChecked()
{
    return m_isChecked;
}

bool DImageButton::isCheckable()
{
    return m_isCheckable;
}

void DImageButton::setNormalPic(const QString &normalPicPixmap)
{
    m_normalPic = normalPicPixmap;
    changeState();
}

void DImageButton::setHoverPic(const QString &hoverPicPixmap)
{
    m_hoverPic = hoverPicPixmap;
    changeState();
}

void DImageButton::setPressPic(const QString &pressPicPixmap)
{
    m_pressPic = pressPicPixmap;
    changeState();
}

void DImageButton::setCheckedPic(const QString &checkedPicPixmap)
{
    m_checkedPic = checkedPicPixmap;
    changeState();
}

DImageButton::State DImageButton::getState() const
{
    return m_state;
}

DWIDGET_END_NAMESPACE
