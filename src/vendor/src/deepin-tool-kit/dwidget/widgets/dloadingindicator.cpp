/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QGraphicsProxyWidget>
#include <QLabel>

#include "dloadingindicator.h"
#include "private/dloadingindicator_p.h"
#include "dthememanager.h"

DWIDGET_BEGIN_NAMESPACE

DLoadingIndicatorPrivate::DLoadingIndicatorPrivate(DLoadingIndicator *qq) :
    DObjectPrivate(qq)
{

}

void DLoadingIndicatorPrivate::init()
{
    D_Q(DLoadingIndicator);

    q->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    q->setScene(new QGraphicsScene(q));

    rotateAni.setDuration(1000);
    rotateAni.setEasingCurve(QEasingCurve::OutInQuad);

    rotateAni.setLoopCount(-1);
    rotateAni.setStartValue(QVariant(qreal(0.0)));
    rotateAni.setEndValue(QVariant(qreal(360.0)));

    q->connect(&rotateAni, SIGNAL(valueChanged(QVariant)), q, SLOT(setRotate(QVariant)));
}

void DLoadingIndicatorPrivate::setLoadingItem(QGraphicsItem *item)
{
    D_QC(DLoadingIndicator);

    QSizeF itemSize = item->boundingRect().size();

    item->setPos((q->width()-itemSize.width())/2,
                 (q->height()-itemSize.height())/2);
    item->setTransformOriginPoint(itemSize.width()/2, itemSize.height()/2);

    q->scene()->clear();
    q->scene()->addItem(item);
}

DLoadingIndicator::DLoadingIndicator(QWidget *parent) :
    QGraphicsView(parent),
    DObject(*new DLoadingIndicatorPrivate(this))
{
    D_THEME_INIT_WIDGET(DLoadingIndicator);

    d_func()->init();
}

DLoadingIndicator::~DLoadingIndicator()
{
    D_DC(DLoadingIndicator);

    d->widgetSource->deleteLater();
}

QColor DLoadingIndicator::backgroundColor() const
{
    return scene()->backgroundBrush().color();
}

void DLoadingIndicator::setRotate(QVariant angle)
{
    if(!scene()->items().isEmpty())
        scene()->items().first()->setRotation(angle.toReal());
}

void DLoadingIndicator::setWidgetSource(QWidget *widgetSource)
{
    D_D(DLoadingIndicator);

    if(d->widgetSource)
        d->widgetSource->deleteLater();

    d->widgetSource = widgetSource;

    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget;

    proxy->setWidget(widgetSource);

    d->setLoadingItem(proxy);
}

void DLoadingIndicator::setImageSource(const QPixmap &imageSource)
{
    D_D(DLoadingIndicator);

    QGraphicsPixmapItem * item = new QGraphicsPixmapItem(imageSource);

    if(d->smooth)
        item->setTransformationMode(Qt::SmoothTransformation);

    d->setLoadingItem(item);
}

void DLoadingIndicator::setAniEasingType(QEasingCurve::Type aniEasingType)
{
    setAniEasingCurve(aniEasingType);
}

void DLoadingIndicator::setSmooth(bool smooth)
{
    D_D(DLoadingIndicator);

    if(d->smooth == smooth)
        return;

    d->smooth = smooth;

    QGraphicsPixmapItem * item = nullptr;

    if(!scene()->items().isEmpty())
        item = dynamic_cast<QGraphicsPixmapItem*>(scene()->items().first());

    if(smooth){
        setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
        if(item)
            item->setTransformationMode(Qt::SmoothTransformation);
    }else{
        setRenderHint(QPainter::SmoothPixmapTransform, false);
        setRenderHint(QPainter::Antialiasing, false);
        if(item)
            item->setTransformationMode(Qt::FastTransformation);
    }
}

void DLoadingIndicator::setDirection(DLoadingIndicator::RotationDirection direction)
{
    D_D(DLoadingIndicator);

    if (d->direction == direction)
        return;

    d->direction = direction;

    if(direction == Clockwise) {
        d->rotateAni.setStartValue(QVariant(qreal(0.0)));
        d->rotateAni.setEndValue(QVariant(qreal(360.0)));
    } else {
        d->rotateAni.setStartValue(QVariant(qreal(0.0)));
        d->rotateAni.setEndValue(QVariant(qreal(-360.0)));
    }

    emit directionChanged(direction);
}

void DLoadingIndicator::resizeEvent(QResizeEvent *e)
{
    QGraphicsView::resizeEvent(e);

    setSceneRect(QRectF(rect()));

    for(QGraphicsItem *item : items()) {
        QSizeF itemSize = item->boundingRect().size();

        item->setPos((width()-itemSize.width())/2,
                                 (height()-itemSize.height())/2);
    }
}

void DLoadingIndicator::setLoading(bool flag)
{
    if (flag == true){
        start();
    } else {
        stop();
    }
}

void DLoadingIndicator::setAniDuration(int msecs)
{
    D_D(DLoadingIndicator);

    d->rotateAni.setDuration(msecs);
}

void DLoadingIndicator::setAniEasingCurve(const QEasingCurve & easing)
{
    D_D(DLoadingIndicator);

    d->rotateAni.setEasingCurve(easing);
}

void DLoadingIndicator::setBackgroundColor(const QColor &color)
{
    scene()->setBackgroundBrush(color);
}

bool DLoadingIndicator::loading() const
{
    D_DC(DLoadingIndicator);

    return d->rotateAni.state() == QVariantAnimation::Running;
}

QWidget *DLoadingIndicator::widgetSource() const
{
    D_DC(DLoadingIndicator);

    return d->widgetSource;
}

QPixmap DLoadingIndicator::imageSource() const
{
    QGraphicsPixmapItem * item = nullptr;

    if(!scene()->items().isEmpty())
        item = dynamic_cast<QGraphicsPixmapItem*>(scene()->items().first());

    return item ? item->pixmap() : QPixmap();
}

int DLoadingIndicator::aniDuration() const
{
    D_DC(DLoadingIndicator);

    return d->rotateAni.duration();
}

QEasingCurve::Type DLoadingIndicator::aniEasingType() const
{
    D_DC(DLoadingIndicator);

    return d->rotateAni.easingCurve().type();
}

QSize DLoadingIndicator::sizeHint() const
{
    return scene()->sceneRect().size().toSize();
}

bool DLoadingIndicator::smooth() const
{
    D_DC(DLoadingIndicator);

    return d->smooth;
}

DLoadingIndicator::RotationDirection DLoadingIndicator::direction() const
{
    D_DC(DLoadingIndicator);

    return d->direction;
}

qreal DLoadingIndicator::rotate() const
{
    if(!scene()->items().isEmpty())
        return scene()->items().first()->rotation();

    return 0;
}

void DLoadingIndicator::start()
{
    D_D(DLoadingIndicator);

    d->rotateAni.start();
}

void DLoadingIndicator::stop()
{
    D_D(DLoadingIndicator);

    d->rotateAni.stop();
}


DWIDGET_END_NAMESPACE
