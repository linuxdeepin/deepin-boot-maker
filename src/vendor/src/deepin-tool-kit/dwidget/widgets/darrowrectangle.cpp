/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "darrowrectangle.h"

#ifdef Q_OS_UNIX
#include <X11/extensions/shape.h>
#include <QX11Info>
#endif
#include <QApplication>
#include <QScreen>
#include <QEvent>

DWIDGET_USE_NAMESPACE

DArrowRectangle::DArrowRectangle(ArrowDirection direction, QWidget * parent) :
    QWidget(parent),m_arrowDirection(direction)
{
    D_THEME_INIT_WIDGET(DArrowRectangle);

    setWindowFlags(Qt::SplashScreen);
    setAttribute(Qt::WA_TranslucentBackground);

    DGraphicsGlowEffect *glowEffect = new DGraphicsGlowEffect(this);
    glowEffect->setBlurRadius(shadowBlurRadius());
    glowEffect->setDistance(shadowDistance());
    glowEffect->setXOffset(shadowXOffset());
    glowEffect->setYOffset(shadowYOffset());
    setGraphicsEffect(glowEffect);
}

void DArrowRectangle::show(int x, int y)
{
    resizeWithContent();

    m_lastPos = QPoint(x, y);
    move(x, y);//Overload function
    if (isHidden())
        QWidget::show();

    update();
}

void DArrowRectangle::setContent(QWidget *content)
{
    if (!content)
        return;

    m_content = content;
    m_content->setParent(this);
    m_content->show();

    qreal delta = shadowBlurRadius() + shadowDistance() + margin();

    resizeWithContent();

    switch(m_arrowDirection)
    {
    case ArrowLeft:
        m_content->move(m_arrowHeight + delta, delta);
        break;
    case ArrowRight:
        m_content->move(delta, delta);
        break;
    case ArrowTop:
        m_content->move(delta, delta + m_arrowHeight);
        break;
    case ArrowBottom:
        m_content->move(delta, delta);
        break;
    }

    update();
}

QWidget *DArrowRectangle::getContent() const
{
    return m_content;
}

void DArrowRectangle::resizeWithContent()
{
    setFixedSize(getFixedSize());

#ifdef Q_OS_UNIX
    XRectangle m_contentXRect;
    m_contentXRect.x = m_content->pos().x();
    m_contentXRect.y = m_content->pos().y();
    m_contentXRect.width = m_content->width();
    m_contentXRect.height = m_content->height();
    XShapeCombineRectangles(QX11Info::display(), winId(), ShapeInput,
                            0,
                            0,
                            &m_contentXRect, 1, ShapeSet, YXBanded);
#endif
}

QSize DArrowRectangle::getFixedSize()
{
    if (m_content)
    {
        qreal delta = shadowBlurRadius() + shadowDistance() + margin();

        switch(m_arrowDirection)
        {
        case ArrowLeft:
        case ArrowRight:
            return QSize(m_content->width() + delta * 2 + m_arrowHeight, m_content->height() + delta * 2);
        case ArrowTop:
        case ArrowBottom:
            return QSize(m_content->width() + delta * 2, m_content->height() + delta * 2 + m_arrowHeight);
        }
    }

    return QSize(0, 0);
}

void DArrowRectangle::move(int x, int y)
{
    switch (m_arrowDirection)
    {
    case ArrowLeft:
    case ArrowRight:
        verticalMove(x, y);
        break;
    case ArrowTop:
    case ArrowBottom:
        horizontalMove(x, y);
        break;
    default:
        QWidget::move(x, y);
        break;
    }
}

// override methods
void DArrowRectangle::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath border;

    switch (m_arrowDirection)
    {
    case DArrowRectangle::ArrowLeft:
        border = getLeftCornerPath();
        break;
    case DArrowRectangle::ArrowRight:
        border = getRightCornerPath();
        break;
    case DArrowRectangle::ArrowTop:
        border = getTopCornerPath();
        break;
    case DArrowRectangle::ArrowBottom:
        border = getBottomCornerPath();
        break;
    default:
        border = getRightCornerPath();
    }

    painter.setClipPath(border);
    painter.fillPath(border, QBrush(m_backgroundColor));

    QPen strokePen;
    strokePen.setColor(m_borderColor);
    strokePen.setWidth(m_borderWidth);
    painter.strokePath(border, strokePen);
}

bool DArrowRectangle::event(QEvent *e)
{
    switch (e->type())
    {
    case QEvent::WindowDeactivate:  emit windowDeactivate();    break;
    default:;
    }

    return QWidget::event(e);
}

const QRect DArrowRectangle::currentScreenRect(const int x, const int y)
{
    for (QScreen *screen : qApp->screens())
        if (screen->geometry().contains(x, y))
            return screen->geometry();

    return QRect();
}

qreal DArrowRectangle::shadowYOffset() const
{
    return m_shadowYOffset;
}

void DArrowRectangle::setShadowYOffset(const qreal &shadowYOffset)
{
    m_shadowYOffset = shadowYOffset;
}

qreal DArrowRectangle::shadowXOffset() const
{
    return m_shadowXOffset;
}

void DArrowRectangle::setShadowXOffset(const qreal &shadowXOffset)
{
    m_shadowXOffset = shadowXOffset;
}

qreal DArrowRectangle::shadowDistance() const
{
    return m_shadowDistance;
}

void DArrowRectangle::setShadowDistance(const qreal &shadowDistance)
{
    m_shadowDistance = shadowDistance;
}

qreal DArrowRectangle::shadowBlurRadius() const
{
    return m_shadowBlurRadius;
}

void DArrowRectangle::setShadowBlurRadius(const qreal &shadowBlurRadius)
{
    m_shadowBlurRadius = shadowBlurRadius;
}

QColor DArrowRectangle::borderColor() const
{
    return m_borderColor;
}

void DArrowRectangle::setBorderColor(const QColor &borderColor)
{
    m_borderColor = borderColor;
}

int DArrowRectangle::borderWidth() const
{
    return m_borderWidth;
}

void DArrowRectangle::setBorderWidth(int borderWidth)
{
    m_borderWidth = borderWidth;
}

QColor DArrowRectangle::backgroundColor() const
{
    return m_backgroundColor;
}

DArrowRectangle::ArrowDirection DArrowRectangle::arrowDirection() const
{
    return m_arrowDirection;
}

void DArrowRectangle::setBackgroundColor(const QColor &backgroundColor)
{
    m_backgroundColor = backgroundColor;
}

int DArrowRectangle::radius() const
{
    return this->m_radius;
}

int DArrowRectangle::arrowHeight() const
{
    return this->m_arrowHeight;
}

int DArrowRectangle::arrowWidth() const
{
    return this->m_arrowWidth;
}

int DArrowRectangle::arrowX() const
{
    return this->m_arrowX;
}

int DArrowRectangle::arrowY() const
{
    return this->m_arrowY;
}

int DArrowRectangle::margin() const
{
    return this->m_margin;
}

void DArrowRectangle::setArrowDirection(ArrowDirection value)
{
    m_arrowDirection = value;
}

void DArrowRectangle::setWidth(int value)
{
    this->setFixedWidth(value);
}

void DArrowRectangle::setHeight(int value)
{
    this->setFixedHeight(value);
}

void DArrowRectangle::setRadius(int value)
{
    this->m_radius = value;
}

void DArrowRectangle::setArrowHeight(int value)
{
    this->m_arrowHeight = value;
}

void DArrowRectangle::setArrowWidth(int value)
{
    this->m_arrowWidth = value;
}

void DArrowRectangle::setArrowX(int value)
{
    this->m_arrowX = value;
}

void DArrowRectangle::setArrowY(int value)
{
    this->m_arrowY = value;
}

void DArrowRectangle::setMargin(int value)
{
    this->m_margin = value;
}

QPainterPath DArrowRectangle::getLeftCornerPath()
{
    qreal delta = shadowBlurRadius() + shadowDistance();

    QRect rect = this->rect().marginsRemoved(QMargins(delta, delta, delta, delta));

    QPoint cornerPoint(rect.x(), rect.y() + (m_arrowY > 0 ? m_arrowY : (rect.height() / 2)));
    QPoint topLeft(rect.x() + m_arrowHeight, rect.y());
    QPoint topRight(rect.x() + rect.width(), rect.y());
    QPoint bottomRight(rect.x() + rect.width(), rect.y() + rect.height());
    QPoint bottomLeft(rect.x() + m_arrowHeight, rect.y() + rect.height());
    int radius = this->m_radius > (rect.height() / 2) ? (rect.height() / 2) : this->m_radius;

    QPainterPath border;
    border.moveTo(topLeft.x() - radius,topLeft.y());
    border.lineTo(topRight.x() - radius, topRight.y());
    border.arcTo(topRight.x() - 2 * radius, topRight.y(), 2 * radius, 2 * radius, 90, -90);
    border.lineTo(bottomRight.x(), bottomRight.y() - radius);
    border.arcTo(bottomRight.x() - 2 * radius, bottomRight.y() - 2 * radius, 2 * radius, 2 * radius, 0, -90);
    border.lineTo(bottomLeft.x() - radius,bottomLeft.y());
    border.arcTo(bottomLeft.x(),bottomLeft.y() - 2 * radius,2 * radius,2 * radius,-90,-90);
    border.lineTo(cornerPoint.x() + m_arrowHeight,cornerPoint.y() + m_arrowWidth / 2);
    border.lineTo(cornerPoint);
    border.lineTo(cornerPoint.x() + m_arrowHeight,cornerPoint.y() - m_arrowWidth / 2);
    border.lineTo(topLeft.x(),topLeft.y() + radius);
    border.arcTo(topLeft.x(),topLeft.y(),2 * radius,2 * radius,-180,-90);
    border.lineTo(topLeft.x() - radius,topLeft.y());

    return border;
}

QPainterPath DArrowRectangle::getRightCornerPath()
{
    qreal delta = shadowBlurRadius() + shadowDistance();

    QRect rect = this->rect().marginsRemoved(QMargins(delta, delta, delta, delta));

    QPoint cornerPoint(rect.x() + rect.width(), rect.y() + (m_arrowY > 0 ? m_arrowY : rect.height() / 2));
    QPoint topLeft(rect.x(), rect.y());
    QPoint topRight(rect.x() + rect.width() - m_arrowHeight, rect.y());
    QPoint bottomRight(rect.x() + rect.width() - m_arrowHeight, rect.y() + rect.height());
    QPoint bottomLeft(rect.x(), rect.y() + rect.height());
    int radius = this->m_radius > (rect.height() / 2) ? rect.height() / 2 : this->m_radius;

    QPainterPath border;
    border.moveTo(topLeft.x() + radius, topLeft.y());
    border.lineTo(topRight.x() - radius,topRight.y());
    border.arcTo(topRight.x() - 2 * radius,topRight.y(),2 * radius,2 * radius,90,-90);
    border.lineTo(cornerPoint.x() - m_arrowHeight,cornerPoint.y() - m_arrowWidth / 2);
    border.lineTo(cornerPoint);
    border.lineTo(cornerPoint.x() - m_arrowHeight,cornerPoint.y() + m_arrowWidth / 2);
    border.lineTo(bottomRight.x(),bottomRight.y() - radius);
    border.arcTo(bottomRight.x() - 2 * radius,bottomRight.y() - 2 * radius,2 * radius,2 * radius,0,-90);
    border.lineTo(bottomLeft.x() + radius, bottomLeft.y());
    border.arcTo(bottomLeft.x(), bottomLeft.y() - 2 * radius, 2 * radius, 2 * radius, -90, -90);
    border.lineTo(topLeft.x(), topLeft.y() + radius);
    border.arcTo(topLeft.x(), topLeft.y(), 2 * radius, 2 * radius, 180, -90);

    return border;
}

QPainterPath DArrowRectangle::getTopCornerPath()
{
    qreal delta = shadowBlurRadius() + shadowDistance();

    QRect rect = this->rect().marginsRemoved(QMargins(delta, delta, delta, delta));

    QPoint cornerPoint(rect.x() + (m_arrowX > 0 ? m_arrowX : rect.width() / 2), rect.y());
    QPoint topLeft(rect.x(), rect.y() + m_arrowHeight);
    QPoint topRight(rect.x() + rect.width(), rect.y() + m_arrowHeight);
    QPoint bottomRight(rect.x() + rect.width(), rect.y() + rect.height());
    QPoint bottomLeft(rect.x(), rect.y() + rect.height());
    int radius = this->m_radius > (rect.height() / 2 - m_arrowHeight) ? rect.height() / 2 -m_arrowHeight : this->m_radius;

    QPainterPath border;
    border.moveTo(topLeft.x() + radius, topLeft.y());
    border.lineTo(cornerPoint.x() - m_arrowWidth / 2, cornerPoint.y() + m_arrowHeight);
    border.lineTo(cornerPoint);
    border.lineTo(cornerPoint.x() + m_arrowWidth / 2, cornerPoint.y() + m_arrowHeight);
    border.lineTo(topRight.x() - radius, topRight.y());
    border.arcTo(topRight.x() - 2 * radius, topRight.y(), 2 * radius, 2 * radius, 90, -90);
    border.lineTo(bottomRight.x(), bottomRight.y() - radius);
    border.arcTo(bottomRight.x() - 2 * radius, bottomRight.y() - 2 * radius, 2 * radius, 2 * radius, 0, -90);
    border.lineTo(bottomLeft.x() + radius, bottomLeft.y());
    border.arcTo(bottomLeft.x(), bottomLeft.y() - 2 * radius, 2 * radius, 2 * radius, - 90, -90);
    border.lineTo(topLeft.x(), topLeft.y() + radius);
    border.arcTo(topLeft.x(), topLeft.y(), 2 * radius, 2 * radius, 180, -90);

    return border;
}

QPainterPath DArrowRectangle::getBottomCornerPath()
{
    qreal delta = shadowBlurRadius() + shadowDistance();

    QRect rect = this->rect().marginsRemoved(QMargins(delta, delta, delta, delta));

    QPoint cornerPoint(rect.x() + (m_arrowX > 0 ? m_arrowX : rect.width() / 2), rect.y()  + rect.height());
    QPoint topLeft(rect.x(), rect.y());
    QPoint topRight(rect.x() + rect.width(), rect.y());
    QPoint bottomRight(rect.x() + rect.width(), rect.y() + rect.height() - m_arrowHeight);
    QPoint bottomLeft(rect.x(), rect.y() + rect.height() - m_arrowHeight);
    int radius = this->m_radius > (rect.height() / 2 - m_arrowHeight) ? rect.height() / 2 -m_arrowHeight : this->m_radius;

    QPainterPath border;
    border.moveTo(topLeft.x() + radius, topLeft.y());
    border.lineTo(topRight.x() - radius, topRight.y());
    border.arcTo(topRight.x() - 2 * radius, topRight.y(), 2 * radius, 2 * radius, 90, -90);
    border.lineTo(bottomRight.x(), bottomRight.y() - radius);
    border.arcTo(bottomRight.x() - 2 * radius, bottomRight.y() - 2 * radius, 2 * radius, 2 * radius, 0, -90);
    border.lineTo(cornerPoint.x() + m_arrowWidth / 2, cornerPoint.y() - m_arrowHeight);
    border.lineTo(cornerPoint);
    border.lineTo(cornerPoint.x() - m_arrowWidth / 2, cornerPoint.y() - m_arrowHeight);
    border.lineTo(bottomLeft.x() + radius, bottomLeft.y());
    border.arcTo(bottomLeft.x(), bottomLeft.y() - 2 * radius, 2 * radius, 2 * radius, -90, -90);
    border.lineTo(topLeft.x(), topLeft.y() + radius);
    border.arcTo(topLeft.x(), topLeft.y(), 2 * radius, 2 * radius, 180, -90);

    return border;
}

void DArrowRectangle::verticalMove(int x, int y)
{
    const QRect dRect = currentScreenRect(x, y);
    qreal delta = shadowBlurRadius() - shadowDistance();

    int lRelativeY = y - dRect.y() - (height() - delta) / 2;
    int rRelativeY = y - dRect.y() + (height() - delta) / 2 - dRect.height();
    int absoluteY = 0;

    if (lRelativeY < 0)//out of screen in top side
    {
        //arrowY use relative coordinates
        setArrowY(height() / 2 - delta + lRelativeY);
        absoluteY = dRect.y() - delta;
    }
    else if(rRelativeY > 0)//out of screen in bottom side
    {
        setArrowY(height() / 2 - delta / 2 + rRelativeY);
        absoluteY = dRect.y() + dRect.height() - height() + delta;
    }
    else
    {
        setArrowY(0);
        absoluteY = y - height() / 2;
    }

    switch (m_arrowDirection)
    {
    case ArrowLeft:
        QWidget::move(x - delta, absoluteY);
        break;
    case ArrowRight:
        QWidget::move(x - width() + delta, absoluteY);
        break;
    default:
        break;
    }
}

void DArrowRectangle::horizontalMove(int x, int y)
{
    const QRect dRect = currentScreenRect(x, y);
    qreal delta = shadowBlurRadius() - shadowDistance();

    int lRelativeX = x - dRect.x() - (width() - delta) / 2;
    int rRelativeX = x - dRect.x() + (width() - delta) / 2 - dRect.width();
    int absoluteX = 0;

    if (lRelativeX < 0)//out of screen in left side
    {
        //arrowX use relative coordinates
        setArrowX((width() - delta) / 2 + lRelativeX);
        absoluteX = dRect.x() - delta;
    }
    else if(rRelativeX > 0)//out of screen in right side
    {
        setArrowX(width() / 2 - delta * 2 + rRelativeX);
        absoluteX = dRect.x() + dRect.width() - width() + delta;
    }
    else
    {
        setArrowX(0);
        absoluteX = x - width() / 2;
    }

    switch (m_arrowDirection)
    {
    case ArrowTop:
        QWidget::move(absoluteX, y - delta);
        break;
    case ArrowBottom:
        QWidget::move(absoluteX, y - height() + delta);
        break;
    default:
        break;
    }
}

DArrowRectangle::~DArrowRectangle()
{

}


