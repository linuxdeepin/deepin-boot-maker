/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DARROWRECTANGLE_H
#define DARROWRECTANGLE_H

#include <QDesktopWidget>
#include <QWidget>
#include <QLabel>
#include <QTextLine>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QTimer>
#include <QDebug>

#include "dwidget_global.h"
#include "dthememanager.h"
#include "dgraphicsgloweffect.h"

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DArrowRectangle : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth)
    Q_PROPERTY(int radius READ radius WRITE setRadius)

public:
    enum ArrowDirection {
        ArrowLeft,
        ArrowRight,
        ArrowTop,
        ArrowBottom
    };

    explicit DArrowRectangle(ArrowDirection direction, QWidget * parent = 0);
    ~DArrowRectangle();

    int radius() const;
    int arrowHeight() const;
    int arrowWidth() const;
    int arrowX() const;
    int arrowY() const;
    int margin() const;
    int borderWidth() const;
    QColor borderColor() const;
    QColor backgroundColor() const;
    ArrowDirection arrowDirection() const;

    void setRadius(int value);
    void setArrowHeight(int value);
    void setArrowWidth(int value);
    void setArrowX(int value);
    void setArrowY(int value);
    void setMargin(int value);
    void setBorderWidth(int borderWidth);
    void setBorderColor(const QColor &borderColor);
    void setBackgroundColor(const QColor &backgroundColor);
    void setArrowDirection(ArrowDirection value);
    void setWidth(int value);
    void setHeight(int value);

    virtual void show(int x, int y);

    void setContent(QWidget *content);
    QWidget *getContent() const;
    void resizeWithContent();
    void move(int x,int y);
    QSize getFixedSize();

    qreal shadowBlurRadius() const;
    void setShadowBlurRadius(const qreal &shadowBlurRadius);

    qreal shadowDistance() const;
    void setShadowDistance(const qreal &shadowDistance);

    qreal shadowXOffset() const;
    void setShadowXOffset(const qreal &shadowXOffset);

    qreal shadowYOffset() const;
    void setShadowYOffset(const qreal &shadowYOffset);

signals:
    void windowDeactivate() const;

protected:
    void paintEvent(QPaintEvent *);
    bool event(QEvent *e);

private:
    const QRect currentScreenRect(const int x, const int y);
    QPainterPath getLeftCornerPath();
    QPainterPath getRightCornerPath();
    QPainterPath getTopCornerPath();
    QPainterPath getBottomCornerPath();

    void verticalMove(int x, int y);
    void horizontalMove(int x, int y);

private:
    int m_radius = 3;
    int m_arrowHeight = 8;
    int m_arrowWidth = 12;
    int m_margin = 5;
    int m_arrowX = 0;
    int m_arrowY = 0;

    qreal m_shadowBlurRadius = 20;
    qreal m_shadowDistance = 1;
    qreal m_shadowXOffset = 0;
    qreal m_shadowYOffset = 5;
    int m_shadowWidth = 5; //Abandoned
    int m_borderWidth = 1;
    QColor m_borderColor = QColor(255,255,255,130);
    QColor m_backgroundColor = QColor(0,0,0,200);

    ArrowDirection m_arrowDirection;
    QPoint m_lastPos = QPoint(0, 0);

    QWidget *m_content = NULL;
};

DWIDGET_END_NAMESPACE

#endif // DARROWRECTANGLE_H
