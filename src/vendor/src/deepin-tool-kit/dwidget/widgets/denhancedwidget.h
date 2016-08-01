/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DENHANCEDWIDGET_H
#define DENHANCEDWIDGET_H

#include <QWidget>

#include "dwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class DEnhancedWidgetPrivate;
class DEnhancedWidget: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QWidget *target READ target WRITE setTarget NOTIFY targetChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)

public:
    explicit DEnhancedWidget(QWidget *target, QObject *parent = 0);
    ~DEnhancedWidget();

    QWidget *target() const;
    bool enabled() const;

public slots:
    void setTarget(QWidget *target);
    void setEnabled(bool enabled);

signals:
    void xChanged(int x);
    void yChanged(int y);
    void positionChanged(const QPoint &point);
    void widthChanged(int width);
    void heightChanged(int height);
    void sizeChanged(const QSize &size);
    void targetChanged(QWidget *target);
    void enabledChanged(bool enabled);

protected:
    bool eventFilter(QObject *o, QEvent *e) Q_DECL_OVERRIDE;

private:
    explicit DEnhancedWidget(DEnhancedWidgetPrivate *dd, QWidget *w, QObject *parent = 0);

    DEnhancedWidgetPrivate *d_ptr;

    Q_DECLARE_PRIVATE(DEnhancedWidget)
};

DWIDGET_END_NAMESPACE

#endif // DENHANCEDWIDGET_H
