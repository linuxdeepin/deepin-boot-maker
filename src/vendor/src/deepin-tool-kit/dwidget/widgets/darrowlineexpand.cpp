/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "darrowlineexpand.h"
#include "dthememanager.h"

#include <QResizeEvent>

DWIDGET_BEGIN_NAMESPACE

ArrowHeaderLine::ArrowHeaderLine(QWidget *parent) :
    DHeaderLine(parent)
{
    m_arrowButton = new DArrowButton(this);
    connect(m_arrowButton, &DArrowButton::mouseRelease, this, &ArrowHeaderLine::mousePress);
    setContent(m_arrowButton);
    setFixedHeight(EXPAND_HEADER_HEIGHT);
}

void ArrowHeaderLine::setExpand(bool value)
{
    if (value)
        m_arrowButton->setArrowDirection(DArrowButton::ArrowUp);
    else
        m_arrowButton->setArrowDirection(DArrowButton::ArrowDown);
}

void ArrowHeaderLine::mousePressEvent(QMouseEvent *)
{
    emit mousePress();
}

void ArrowHeaderLine::reverseArrowDirection()
{
    if (m_arrowButton->arrowDirection() == DArrowButton::ArrowUp)
        m_arrowButton->setArrowDirection(DArrowButton::ArrowDown);
    else
        m_arrowButton->setArrowDirection(DArrowButton::ArrowUp);
}

DArrowLineExpand::DArrowLineExpand(QWidget *parent) : DBaseExpand(parent)
{
    m_headerLine = new ArrowHeaderLine(this);
    m_headerLine->setExpand(expand());
    connect(m_headerLine, &ArrowHeaderLine::mousePress, [=]{
        setExpand(!expand());
    });
    setHeader(m_headerLine);
}

void DArrowLineExpand::setTitle(const QString &title)
{
    m_headerLine->setTitle(title);
}

void DArrowLineExpand::setExpand(bool value)
{
    //Header's arrow direction change here
    m_headerLine->setExpand(value);
    DBaseExpand::setExpand(value);
}

DBaseLine *DArrowLineExpand::headerLine()
{
    return m_headerLine;
}

void DArrowLineExpand::setHeader(QWidget *header)
{
    DBaseExpand::setHeader(header);
}

void DArrowLineExpand::resizeEvent(QResizeEvent *e)
{
    m_headerLine->setFixedWidth(e->size().width());

    DBaseExpand::resizeEvent(e);
}

DWIDGET_END_NAMESPACE
