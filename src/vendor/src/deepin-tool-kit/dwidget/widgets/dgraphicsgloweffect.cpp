/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dgraphicsgloweffect.h"

DWIDGET_USE_NAMESPACE

DGraphicsGlowEffect::DGraphicsGlowEffect(QObject *parent) :
    QGraphicsEffect(parent),
    m_xOffset(0),
    m_yOffset(0),
    m_distance(4.0f),
    m_blurRadius(10.0f),
    m_color(0, 0, 0, 80)
{
}

QT_BEGIN_NAMESPACE
  extern Q_WIDGETS_EXPORT void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0 );
QT_END_NAMESPACE

void DGraphicsGlowEffect::draw(QPainter* painter)
{
    // if nothing to show outside the item, just draw source
    if ((blurRadius() + distance()) <= 0) {
        drawSource(painter);
        return;
    }

    PixmapPadMode mode = QGraphicsEffect::PadToEffectiveBoundingRect;
    QPoint offset;
    const QPixmap sourcePx = sourcePixmap(Qt::DeviceCoordinates, &offset, mode);

    // return if no source
    if (sourcePx.isNull())
        return;

    // save world transform
    QTransform restoreTransform = painter->worldTransform();
    painter->setWorldTransform(QTransform());

    // Calculate size for the background image
    QSize scaleSize(sourcePx.size().width() + 2 * distance(), sourcePx.size().height() + 2 * distance());

    QImage tmpImg(scaleSize, QImage::Format_ARGB32_Premultiplied);
    QPixmap scaled = sourcePx.scaled(scaleSize);
    tmpImg.fill(0);
    QPainter tmpPainter(&tmpImg);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_Source);
    tmpPainter.drawPixmap(QPointF(-distance(), -distance()), scaled);
    tmpPainter.end();

    // blur the alpha channel
    QImage blurred(tmpImg.size(), QImage::Format_ARGB32_Premultiplied);
    blurred.fill(0);
    QPainter blurPainter(&blurred);
    qt_blurImage(&blurPainter, tmpImg, blurRadius(), false, true);
    blurPainter.end();

    tmpImg = blurred;

    // blacken the image...
    tmpPainter.begin(&tmpImg);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    tmpPainter.fillRect(tmpImg.rect(), color());
    tmpPainter.end();

    // draw the blurred shadow...
    painter->drawImage(offset, tmpImg);

    // draw the actual pixmap...
    painter->drawPixmap(offset, sourcePx, QRectF());

    // restore world transform
    painter->setWorldTransform(restoreTransform);
}

QRectF DGraphicsGlowEffect::boundingRectFor(const QRectF& rect) const
{
    qreal delta = blurRadius() + distance();
    return rect.united(rect.adjusted(-delta - xOffset(), -delta - yOffset(), delta - xOffset(), delta - yOffset()));
}
