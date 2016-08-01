/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dcircleprogress.h"
#include "private/dcircleprogress_p.h"

#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <QVBoxLayout>

DWIDGET_USE_NAMESPACE

DCircleProgressPrivate::DCircleProgressPrivate(DCircleProgress *q)
    : DObjectPrivate(q)
{
    m_topLabel.setAttribute(Qt::WA_TranslucentBackground);
    m_topLabel.setAlignment(Qt::AlignCenter);
    m_topLabel.setObjectName("TopLabel");
    m_bottomLabel.setAttribute(Qt::WA_TranslucentBackground);
    m_bottomLabel.setAlignment(Qt::AlignCenter);
    m_bottomLabel.setObjectName("BottomLabel");
    m_bottomLabel.hide();

    QBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addStretch();
    mainLayout->addWidget(&m_topLabel);
    mainLayout->addWidget(&m_bottomLabel);
    mainLayout->addStretch();
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);

    q->setLayout(mainLayout);
}

void DCircleProgressPrivate::paint(QPainter *painter)
{
    Q_Q(const DCircleProgress);

    painter->setRenderHints(QPainter::Antialiasing);

    QPen pen(m_chunkColor);
    pen.setWidth(m_lineWidth);
    painter->setPen(pen);
    const QRect widgetRect = q->rect();
    QRect outerCircleRect = widgetRect;
    outerCircleRect.setWidth(outerCircleRect.width() - (m_lineWidth - 1) * 2);
    outerCircleRect.setHeight(outerCircleRect.height() - (m_lineWidth - 1) * 2);
    if (outerCircleRect.width() < outerCircleRect.height())
        outerCircleRect.setHeight(outerCircleRect.width());
    else
        outerCircleRect.setWidth(outerCircleRect.height());
    outerCircleRect.setTop((widgetRect.height() - outerCircleRect.height()) / 2);
    outerCircleRect.setLeft((widgetRect.width() - outerCircleRect.width()) / 2);

    const double percent = double(m_currentValue) / (m_maximumValue - m_minmumValue);
    const int splitPos = -percent * 16 * 360;
    painter->drawArc(outerCircleRect, 90 * 16, splitPos);
    pen.setColor(m_backgroundColor);
    painter->setPen(pen);
    painter->drawArc(outerCircleRect, 90 * 16, 16 * 360 + splitPos);
}

DCircleProgress::DCircleProgress(QWidget *parent)
    : QWidget(parent),
      DObject(*new DCircleProgressPrivate(this))
{
    D_THEME_INIT_WIDGET(DCircleProgress);
}

int DCircleProgress::value() const
{
    Q_D(const DCircleProgress);

    return d->m_currentValue;
}

void DCircleProgress::setValue(int value)
{
    Q_D(DCircleProgress);

    d->m_currentValue = value;
    update();
}

const QString DCircleProgress::text() const
{
    Q_D(const DCircleProgress);

    return d->m_topLabel.text();
}

void DCircleProgress::setText(const QString &text)
{
    Q_D(DCircleProgress);

    return d->m_topLabel.setText(text);
}

const QColor DCircleProgress::backgroundColor() const
{
    Q_D(const DCircleProgress);

    return d->m_backgroundColor;
}

void DCircleProgress::setBackgroundColor(const QColor &color)
{
    Q_D(DCircleProgress);

    d->m_backgroundColor = color;
}

const QColor DCircleProgress::chunkColor() const
{
    Q_D(const DCircleProgress);

    return d->m_chunkColor;
}

void DCircleProgress::setChunkColor(const QColor &color)
{
    Q_D(DCircleProgress);

    d->m_chunkColor = color;
}

int DCircleProgress::lineWidth() const
{
    Q_D(const DCircleProgress);

    return d->m_lineWidth;
}

void DCircleProgress::setLineWidth(const int width)
{
    Q_D(DCircleProgress);

    d->m_lineWidth = width;
}

QLabel *DCircleProgress::topLabel()
{
    Q_D(DCircleProgress);

    return &d->m_topLabel;
}

QLabel *DCircleProgress::bottomLabel()
{
    Q_D(DCircleProgress);

    return &d->m_bottomLabel;
}

void DCircleProgress::paintEvent(QPaintEvent *e)
{
    Q_D(DCircleProgress);

    QPainter p(this);
    d->paint(&p);

    QWidget::paintEvent(e);
}

void DCircleProgress::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e)

    emit clicked();
}

void DCircleProgress::enterEvent(QEvent *e)
{
    emit mouseEntered();

    QWidget::enterEvent(e);
}

void DCircleProgress::leaveEvent(QEvent *e)
{
    emit mouseLeaved();

    QWidget::leaveEvent(e);
}

