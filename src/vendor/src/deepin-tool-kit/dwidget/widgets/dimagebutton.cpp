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
    updateIcon();
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

    updateIcon();
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

    updateIcon();
}

DImageButton::~DImageButton()
{
}

void DImageButton::enterEvent(QEvent *event)
{
    setCursor(Qt::PointingHandCursor);

    if (!m_isChecked){
        setState(Hover);
    }

    event->accept();
    //QLabel::enterEvent(event);
}

void DImageButton::leaveEvent(QEvent *event)
{
    if (!m_isChecked){
        setState(Normal);
    }

    event->accept();
    //QLabel::leaveEvent(event);
}

void DImageButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    setState(Press);

    event->accept();
    //QLabel::mousePressEvent(event);
}

void DImageButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (!rect().contains(event->pos()))
        return;

    if (m_isCheckable){
        m_isChecked = !m_isChecked;
        if (m_isChecked){
            setState(Checked);
        } else {
            setState(Normal);
        }
    } else {
        setState(Hover);
    }

    event->accept();
    //QLabel::mouseReleaseEvent(event);

    if (event->button() == Qt::LeftButton)
        emit clicked();
}

void DImageButton::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_isCheckable && !rect().contains(event->pos())) {
        setState(Normal);
    }
}

void DImageButton::updateIcon()
{
    switch (m_state) {
    case Hover:     if (!m_hoverPic.isEmpty()) setPixmap(QPixmap(m_hoverPic));      break;
    case Press:     if (!m_pressPic.isEmpty()) setPixmap(QPixmap(m_pressPic));      break;
    case Checked:   if (!m_checkedPic.isEmpty()) setPixmap(QPixmap(m_checkedPic));  break;
    default:        if (!m_normalPic.isEmpty()) setPixmap(QPixmap(m_normalPic));    break;
    }

    emit stateChanged();
}

void DImageButton::setState(DImageButton::State state)
{
    if (m_state == state)
        return;

    m_state = state;

    updateIcon();
}

void DImageButton::setCheckable(bool flag)
{
    m_isCheckable = flag;

    if (!m_isCheckable){
        setState(Normal);
    }
}

void DImageButton::setChecked(bool flag)
{
    if (m_isCheckable == false){
        return;
    }

    m_isChecked = flag;
    if (m_isChecked){
        setState(Checked);
    } else {
        setState(Normal);
    }
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
    updateIcon();
}

void DImageButton::setHoverPic(const QString &hoverPicPixmap)
{
    m_hoverPic = hoverPicPixmap;
    updateIcon();
}

void DImageButton::setPressPic(const QString &pressPicPixmap)
{
    m_pressPic = pressPicPixmap;
    updateIcon();
}

void DImageButton::setCheckedPic(const QString &checkedPicPixmap)
{
    m_checkedPic = checkedPicPixmap;
    updateIcon();
}

DImageButton::State DImageButton::getState() const
{
    return m_state;
}

DWIDGET_END_NAMESPACE
