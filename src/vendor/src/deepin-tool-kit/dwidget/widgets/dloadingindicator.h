/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DLOADINGINDICATOR_H
#define DLOADINGINDICATOR_H

#include <QObject>
#include <QBrush>
#include <QColor>
#include <QVariant>
#include <QSizePolicy>
#include <QVariantAnimation>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QEasingCurve>
#include <QDebug>

#include "dwidget_global.h"
#include "dobject.h"

DWIDGET_BEGIN_NAMESPACE

class DLoadingIndicatorPrivate;
class LIBDTKWIDGETSHARED_EXPORT DLoadingIndicator : public QGraphicsView, public DObject
{
    Q_OBJECT

    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY(bool loading READ loading WRITE setLoading)
    Q_PROPERTY(bool smooth READ smooth WRITE setSmooth)
    Q_PROPERTY(QPixmap imageSource READ imageSource WRITE setImageSource)
    Q_PROPERTY(QWidget* widgetSource READ widgetSource WRITE setWidgetSource)
    Q_PROPERTY(int aniDuration READ aniDuration WRITE setAniDuration)
    Q_PROPERTY(QEasingCurve::Type aniEasingType READ aniEasingType WRITE setAniEasingType)
    Q_PROPERTY(RotationDirection direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(qreal rotate READ rotate WRITE setRotate NOTIFY rotateChanged)

public:
    enum RotationDirection{
        Clockwise,
        Counterclockwise
    };

    Q_ENUMS(RotationDirection)

    DLoadingIndicator(QWidget * parent = 0);
    ~DLoadingIndicator();

    QColor backgroundColor() const;
    bool loading() const;
    QWidget* widgetSource() const;
    QPixmap imageSource() const;
    int aniDuration() const;
    QEasingCurve::Type aniEasingType() const;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    bool smooth() const;
    RotationDirection direction() const;
    qreal rotate() const;

public slots:
    void start();
    void stop();
    void setLoading(bool flag);
    void setAniDuration(int msecs);
    void setAniEasingCurve(const QEasingCurve & easing);
    void setBackgroundColor(const QColor &color);
    void setRotate(QVariant angle);
    void setWidgetSource(QWidget* widgetSource);
    void setImageSource(const QPixmap &imageSource);
    void setAniEasingType(QEasingCurve::Type aniEasingType);
    void setSmooth(bool smooth);
    void setDirection(RotationDirection direction);

signals:
    void directionChanged(RotationDirection direction);
    void rotateChanged(qreal rotate);

protected:
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;

private:
    D_DECLARE_PRIVATE(DLoadingIndicator)
};

DWIDGET_END_NAMESPACE

#endif // DLOADINGINDICATOR_H
