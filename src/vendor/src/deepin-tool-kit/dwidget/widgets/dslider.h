/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DSLIDER_H
#define DSLIDER_H

#include <QSlider>

#include "dwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class DSliderPrivate;
class LIBDTKWIDGETSHARED_EXPORT DSlider : public QSlider
{
    Q_OBJECT
    Q_PROPERTY(int handleType READ handleType)
    Q_PROPERTY(QColor tipColor READ tipColor WRITE setTipColor)
    Q_PROPERTY(QColor scaleColor READ scaleColor WRITE setScaleColor)
    Q_PROPERTY(bool hoverShowValue READ hoverShowValue WRITE setHoverShowValue)
    Q_PROPERTY(QColor hoverValueColor READ hoverValueColor WRITE setHoverValueColor)
    Q_PROPERTY(int hoverShowValueInterval READ hoverShowValueInterval WRITE setHoverShowValueInterval)

public:
    enum HandleType {
        SharpHandler,
        RoundHandle
    };

    DSlider(QWidget *parent = 0);
    DSlider(Qt::Orientation orientation, QWidget *parent = 0);
    ~DSlider();

    int handleType() const;
    void setHandleType(HandleType handleType);

    QString leftTip() const;
    void setLeftTip(const QString &leftTip);

    QString rightTip() const;
    void setRightTip(const QString &rightTip);

    QColor tipColor() const;
    void setTipColor(const QColor &tipColor);

    QColor scaleColor() const;
    void setScaleColor(const QColor &scaleColor);

    void addScale(int value);
    void removeScale(int value);

    QSize sizeHint() const;
    bool hoverShowValue() const;
    QColor hoverValueColor() const;
    int hoverShowValueInterval() const;

public slots:
    void setHoverShowValue(bool hoverShowValue);
    void setHoverValueColor(QColor hoverValueColor);
    void setHoverShowValueInterval(int hoverShowValueInterval);

protected:
    DSlider(DSliderPrivate &d);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void hoverTimout();

private:
    DSliderPrivate *d_ptr;

    void init();

    Q_DECLARE_PRIVATE(DSlider)
    Q_DISABLE_COPY(DSlider)
};

DWIDGET_END_NAMESPACE

#endif // DSLIDER_H
