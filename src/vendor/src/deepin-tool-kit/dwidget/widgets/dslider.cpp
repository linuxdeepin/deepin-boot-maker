/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QTimer>
#include <QStyle>
#include <QStyleOptionSlider>

#include "dslider.h"
#include "dthememanager.h"

DWIDGET_BEGIN_NAMESPACE

static const int CustomDrawingLeftPadding = 10;
static const int CustomDrawingRightPadding = 10;
static const int CustomDrawingScaleHeight = 6;

// TODO: optimize for better vertical slider support
class DSliderPrivate
{
    DSliderPrivate(DSlider *q):
        m_hoverShowValue(false),
        m_handleHovering(false),
        m_hoverTimout(false),
        m_hoverShowValueInterval(-1),
        q_ptr(q)
    {
        m_hoverTimer.setSingleShot(true);
    }

    int m_handleType = DSlider::RoundHandle;

    QString m_leftTip;
    QString m_rightTip;
    QList<int> m_scales;

    QColor m_tipColor = Qt::black;
    QColor m_scaleColor = Qt::black;
    QColor m_hoverValueColor;
    bool m_hoverShowValue;
    bool m_handleHovering;
    bool m_hoverTimout;
    bool mousePressed = false;
    int clickOffset = 0;
    int m_hoverShowValueInterval;
    QTimer m_hoverTimer;

    int getScalePosition(int value);
    int pixelPosToRangeValue(int pos) const;

    inline int pick(const QPoint &pt) const
    {
        Q_Q(const DSlider);
        return q->orientation() == Qt::Horizontal ? pt.x() : pt.y();
    }

    DSlider *q_ptr;
    Q_DECLARE_PUBLIC(DSlider)
};

DSlider::DSlider(QWidget *parent) :
    QSlider(parent),
    d_ptr(new DSliderPrivate(this))
{
    init();
}

DSlider::DSlider(Qt::Orientation orientation, QWidget *parent) :
    QSlider(orientation, parent),
    d_ptr(new DSliderPrivate(this))
{
    init();
}

DSlider::~DSlider()
{
    delete d_ptr;
}

int DSlider::handleType() const
{
    Q_D(const DSlider);

    return d->m_handleType;
}

void DSlider::setHandleType(HandleType handleType)
{
    Q_D(DSlider);

    d->m_handleType = handleType;
}


QString DSlider::rightTip() const
{
    Q_D(const DSlider);

    return d->m_rightTip;
}

void DSlider::setRightTip(const QString &rightTip)
{
    Q_D(DSlider);

    if (d->m_rightTip == rightTip) {
        return;
    }

    if (d->m_rightTip.isEmpty() || rightTip.isEmpty()) {
        d->m_rightTip = rightTip;
        updateGeometry();
    } else {
        d->m_rightTip = rightTip;
    }

    repaint();
}

QString DSlider::leftTip() const
{
    Q_D(const DSlider);

    return d->m_leftTip;
}

void DSlider::setLeftTip(const QString &leftTip)
{
    Q_D(DSlider);

    if (d->m_leftTip == leftTip) {
        return;
    }

    if (d->m_leftTip.isEmpty() || leftTip.isEmpty()) {
        d->m_leftTip = leftTip;
        updateGeometry();
    } else {
        d->m_leftTip = leftTip;
    }

    repaint();
}

QColor DSlider::scaleColor() const
{
    Q_D(const DSlider);

    return d->m_scaleColor;
}

void DSlider::setScaleColor(const QColor &scaleColor)
{
    Q_D(DSlider);

    d->m_scaleColor = scaleColor;

    repaint();
}

QColor DSlider::tipColor() const
{
    Q_D(const DSlider);

    return d->m_tipColor;
}

void DSlider::setTipColor(const QColor &tipColor)
{
    Q_D(DSlider);

    d->m_tipColor = tipColor;

    repaint();
}

void DSlider::addScale(int value)
{
    Q_D(DSlider);

    d->m_scales.append(value);

    if (d->m_scales.count() == 1) {
        updateGeometry();
    }

    repaint();
}

void DSlider::removeScale(int value)
{
    Q_D(DSlider);

    d->m_scales.removeOne(value);

    if (d->m_scales.isEmpty()) {
        updateGeometry();
    }

    repaint();
}

void DSlider::mousePressEvent(QMouseEvent *event)
{
    QAbstractSlider::mousePressEvent(event);

    if (event->button() == Qt::LeftButton) {
        if (orientation() == Qt::Vertical) {
            setValue(minimum() + ((maximum() - minimum()) * (height() - event->y())) / height()) ;
        } else {
            // FIXME
            // the value 10 is specified in DSlider.theme, it's ugly here, but I don't have any
            // good idea for now, maybe someone can help.
            setValue(minimum() + ((maximum() - minimum()) * (event->x() - 10)) / (width() - 10 - 10)) ;
        }

        event->accept();

        Q_D(DSlider);

        QStyleOptionSlider opt;
        initStyleOption(&opt);
        setRepeatAction(SliderNoAction);
        QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);

        d->clickOffset = d->pick(event->pos() - sr.topLeft());
        d->mousePressed = true;

        emit sliderPressed();
    }
}

void DSlider::mouseReleaseEvent(QMouseEvent *event)
{
    QAbstractSlider::mouseReleaseEvent(event);

    if(event->button() == Qt::LeftButton) {
        d_func()->mousePressed = false;

        emit sliderReleased();
    }
}

void DSlider::paintEvent(QPaintEvent *event)
{
    Q_D(DSlider);

    QPainter painter;
    painter.begin(this);

    // draw tips
    QFont font = painter.font();
    font.setPixelSize(12);
    painter.setFont(font);

    QPen pen = painter.pen();
    pen.setColor(d->m_tipColor);
    painter.setPen(pen);

    QRect tmp = rect().adjusted(CustomDrawingLeftPadding - 5, 0, -CustomDrawingRightPadding + 5, 0);

    QTextOption leftBottomOption;
    leftBottomOption.setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    painter.drawText(tmp, d->m_leftTip, leftBottomOption);

    QTextOption rightBottomOption;
    rightBottomOption.setAlignment(Qt::AlignRight | Qt::AlignBottom);
    painter.drawText(tmp, d->m_rightTip, rightBottomOption);

    // draw scales
    pen.setColor(d->m_scaleColor);
    painter.setPen(pen);

    foreach(int scale, d->m_scales) {
        int x = d->getScalePosition(scale);
        int y = height() - 8;
        painter.drawLine(x, y, x, y - CustomDrawingScaleHeight);
    }

    if (d->m_handleHovering && !d->m_hoverTimout) {
        QString str = QString::number(value());
        int x = d->getScalePosition(value()) - painter.fontMetrics().width(str) / 2.0;
        painter.setPen(d->m_hoverValueColor);
        painter.drawText(x, 10, str);
    }

    painter.end();

    QSlider::paintEvent(event);
}

void DSlider::mouseMoveEvent(QMouseEvent *event)
{
    QAbstractSlider::mouseMoveEvent(event);

    Q_D(DSlider);

    if(d->mousePressed) {
        int newPosition = d->pixelPosToRangeValue(d->pick(event->pos()) - d->clickOffset);
        setSliderPosition(newPosition);

        emit sliderMoved(newPosition);
    }

    if (!d->m_hoverShowValue) {
        return;
    }

    QPoint pos = event->pos();
    QRect rect(d->getScalePosition(value()) - CustomDrawingLeftPadding, 10, 20, 20);
    if (d->m_handleHovering) {
        d->m_handleHovering = rect.contains(pos);
    } else {
        d->m_handleHovering = rect.contains(pos);
        if (d->m_handleHovering) {
            d->m_hoverTimout = false;
            if (d->m_hoverShowValueInterval > 0) {
                d->m_hoverTimer.start(d->m_hoverShowValueInterval);
            }
        }
    }

    update();
}

void DSlider::hoverTimout()
{
    Q_D(DSlider);

    d->m_hoverTimout = true;
    update();
}

void DSlider::init()
{
    Q_D(DSlider);

    D_THEME_INIT_WIDGET(DSlider);

    setMouseTracking(true);
    connect(&d->m_hoverTimer, &QTimer::timeout, this, &DSlider::hoverTimout);
}

QSize DSlider::sizeHint() const
{
    Q_D(const DSlider);

    QSize size = QSlider::sizeHint();
    if (!d->m_leftTip.isEmpty() || !d->m_rightTip.isEmpty() || !d->m_scales.isEmpty()) {
        size.setHeight(size.height() + 25);
    } else {
        if (d->m_hoverShowValue) {
            size.setHeight(size.height() + 25);
        } else {
            size.setHeight(size.height() + 3);
        }
    }

    return size;
}

bool DSlider::hoverShowValue() const
{
    Q_D(const DSlider);

    return d->m_hoverShowValue;
}

QColor DSlider::hoverValueColor() const
{
    Q_D(const DSlider);

    return d->m_hoverValueColor;
}

int DSlider::hoverShowValueInterval() const
{
    Q_D(const DSlider);

    return d->m_hoverShowValueInterval;
}

void DSlider::setHoverShowValue(bool hoverShowValue)
{
    Q_D(DSlider);

    if (d->m_hoverShowValue == hoverShowValue) {
        return;
    }

    d->m_hoverShowValue = hoverShowValue;
    d->m_handleHovering &= hoverShowValue;

    updateGeometry();
    repaint();
}

void DSlider::setHoverValueColor(QColor hoverValueColor)
{
    Q_D(DSlider);

    d->m_hoverValueColor = hoverValueColor;
}

void DSlider::setHoverShowValueInterval(int hoverShowValueInterval)
{
    Q_D(DSlider);

    d->m_hoverShowValueInterval = hoverShowValueInterval;
}

DSlider::DSlider(DSliderPrivate &d): d_ptr(&d)
{
    init();
}

int DSliderPrivate::getScalePosition(int value)
{
    Q_Q(DSlider);

    float valueRange = q->maximum() - q->minimum();
    float posRange = q->width() - CustomDrawingLeftPadding - CustomDrawingRightPadding;
    return CustomDrawingLeftPadding + (value - q->minimum()) * posRange / valueRange;
}

int DSliderPrivate::pixelPosToRangeValue(int pos) const
{
    Q_Q(const DSlider);

    QStyleOptionSlider opt;
    q->initStyleOption(&opt);
    QRect gr = q->style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, q);
    QRect sr = q->style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, q);
    int sliderMin, sliderMax, sliderLength;

    if (q->orientation() == Qt::Horizontal) {
        sliderLength = sr.width();
        sliderMin = gr.x();
        sliderMax = gr.right() - sliderLength + 1;
    } else {
        sliderLength = sr.height();
        sliderMin = gr.y();
        sliderMax = gr.bottom() - sliderLength + 1;
    }
    return QStyle::sliderValueFromPosition(q->minimum(), q->maximum(), pos - sliderMin,
                                           sliderMax - sliderMin, opt.upsideDown);
}

DWIDGET_END_NAMESPACE
