/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QPainter>
#include <QVideoWidget>
#include <QVideoSurfaceFormat>
#include <QAbstractVideoBuffer>

#include "dcameraview.h"

CameraFormatProxy::CameraFormatProxy(QObject *parent):
    QAbstractVideoSurface(parent)
{
}

QVideoFrame& CameraFormatProxy::currentFrame() const
{
    return const_cast<QVideoFrame&>(m_currentFrame);
}

bool CameraFormatProxy::present(const QVideoFrame &frame)
{
    m_currentFrame = frame;
    emit currentFrameChanged();
    return true;
}

QList<QVideoFrame::PixelFormat> CameraFormatProxy::supportedPixelFormats(QAbstractVideoBuffer::HandleType) const
{
    return QList<QVideoFrame::PixelFormat>()
                     << QVideoFrame::Format_RGB32
                     << QVideoFrame::Format_ARGB32
                     << QVideoFrame::Format_ARGB32_Premultiplied
                     << QVideoFrame::Format_RGB565
                     << QVideoFrame::Format_RGB555;
}

bool CameraFormatProxy::isFormatSupported(const QVideoSurfaceFormat &format) const
{
    return QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat())!=QImage::Format_Invalid;
}


DCameraView::DCameraView(QWidget *parent) :
    QFrame(parent),
    m_source(NULL),
    m_mirroredHorizontal(false),
    m_mirroredVertical(false),
    m_scale(1.0),
    m_aspectRatioMode(Qt::KeepAspectRatio),
    m_brightness(0),
    m_contrast(0),
    m_hue(0),
    m_saturation(0),
    m_round(false)
{
    cameraFormatProxy = new CameraFormatProxy(this);
    connect(cameraFormatProxy, SIGNAL(currentFrameChanged()), this, SLOT(repaint()));
}

QCamera* DCameraView::source() const
{
    return m_source;
}

bool DCameraView::mirroredHorizontal() const
{
    return m_mirroredHorizontal;
}

bool DCameraView::mirroredVertical() const
{
    return m_mirroredVertical;
}

void DCameraView::paint(const QVideoFrame &frame)
{
    QPainter painter(this);

    QImage image(
                frame.bits(),
                frame.width(),
                frame.height(),
                frame.bytesPerLine(),
                QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat()));

    painter.drawImage(0, 0, image.mirrored(m_mirroredHorizontal, m_mirroredVertical));
}

qreal DCameraView::scale() const
{
    return m_scale;
}

Qt::AspectRatioMode DCameraView::aspectRatioMode() const
{
    return m_aspectRatioMode;
}

int DCameraView::brightness() const
{
    return m_brightness;
}

int DCameraView::contrast() const
{
    return m_contrast;
}

int DCameraView::hue() const
{
    return m_hue;
}

int DCameraView::saturation() const
{
    return m_saturation;
}

const QVideoFrame *DCameraView::currentFrame() const
{
    if(cameraFormatProxy){
        return &cameraFormatProxy->currentFrame();
    }else{
        return NULL;
    }
}

//! 返回当前帧
//!
QPixmap DCameraView::capture()
{
    return grab();
}

//! 获取视频截图
//!
bool DCameraView::round() const
{
    return m_round;
}

void DCameraView::setSource(QCamera *source)
{
    if (m_source == source)
        return;

    m_source = source;
    m_source->setCaptureMode(QCamera::CaptureStillImage);
    m_source->setViewfinder(cameraFormatProxy);

    emit sourceChanged(source);
}

void DCameraView::setMirroredHorizontal(bool mirroredHorizontal)
{
    if (m_mirroredHorizontal == mirroredHorizontal)
        return;

    m_mirroredHorizontal = mirroredHorizontal;
    emit mirroredHorizontalChanged(mirroredHorizontal);
}

void DCameraView::setMirroredVertical(bool mirroredVertical)
{
    if (m_mirroredVertical == mirroredVertical)
        return;

    m_mirroredVertical = mirroredVertical;
    emit mirroredVerticalChanged(mirroredVertical);
}

void DCameraView::setScale(qreal scale)
{
    if (m_scale == scale)
        return;

    m_scale = scale;
    emit scaleChanged(scale);
}

void DCameraView::setAspectRatioMode(Qt::AspectRatioMode mode)
{
    m_aspectRatioMode = mode;
}

void DCameraView::setBrightness(int brightness)
{
    if(m_brightness == brightness)
        return;
}

void DCameraView::setContrast(int contrast)
{
    if(m_contrast == contrast)
        return;
}

void DCameraView::setHue(int hue)
{
    if(m_hue == hue)
        return;
}

void DCameraView::setSaturation(int saturation)
{
    if(m_saturation == saturation)
        return;
}

void DCameraView::setRound(bool round)
{
    if (m_round == round)
        return;

    m_round = round;
    emit roundChanged(round);
}

void DCameraView::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    QVideoFrame &frame = cameraFormatProxy->currentFrame();

    frame.map(QAbstractVideoBuffer::ReadOnly);
    QImage image(
                frame.bits(),
                frame.width(),
                frame.height(),
                QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat()));
    frame.unmap();

    if(image.isNull())
        return;

    image = image.scaled(size()*m_scale, m_aspectRatioMode, Qt::SmoothTransformation);
    image = image.mirrored(m_mirroredHorizontal, m_mirroredVertical);

    if(m_round){
        QPainterPath path;
        int diameter = qMin(width(), height());
        path.addEllipse(width()/2.0-diameter/2.0, height()/2.0-diameter/2.0, diameter, diameter);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        painter.setClipPath(path);
    }

    painter.drawImage(rect().center() - image.rect().center(), image);
}


