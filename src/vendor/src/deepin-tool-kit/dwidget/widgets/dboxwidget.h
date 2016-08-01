/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DBOXWIDGET_H
#define DBOXWIDGET_H

#include "dwidget_global.h"

#include <dobject.h>

#include <QFrame>
#include <QBoxLayout>

DWIDGET_BEGIN_NAMESPACE

class DBoxWidgetPrivate;
class DBoxWidget : public QFrame, public DObject
{
    Q_OBJECT

    Q_PROPERTY(QBoxLayout::Direction direction READ direction WRITE setDirection NOTIFY directionChanged)

public:
    explicit DBoxWidget(QBoxLayout::Direction direction, QWidget *parent = 0);

    QBoxLayout::Direction direction() const;
    QBoxLayout *layout() const;

    void addWidget(QWidget *widget);
    QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:
    void setDirection(QBoxLayout::Direction direction);

signals:
    void sizeChanged(QSize size);
    void directionChanged(QBoxLayout::Direction direction);

protected:
    virtual void updateSize(const QSize &size);
    bool event(QEvent *ee) Q_DECL_OVERRIDE;

private:
    D_DECLARE_PRIVATE(DBoxWidget)
};

class DHBoxWidget : public DBoxWidget
{
    Q_OBJECT
public:
    explicit DHBoxWidget(QWidget *parent = 0);
};

class DVBoxWidget : public DBoxWidget
{
    Q_OBJECT
public:
    explicit DVBoxWidget(QWidget *parent = 0);
};

DWIDGET_END_NAMESPACE

#endif // DBOXWIDGET_H
