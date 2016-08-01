/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DSCROLLBAR_H
#define DSCROLLBAR_H

#include <QScrollBar>

#include "dwidget_global.h"
#include "dobject.h"

DWIDGET_BEGIN_NAMESPACE

class DScrollBarPrivate;
class LIBDTKWIDGETSHARED_EXPORT DScrollBar : public QScrollBar, public DObject
{
    Q_OBJECT

    Q_PROPERTY(bool autoHide READ autoHide WRITE setAutoHide NOTIFY autoHideChanged)

public:
    explicit DScrollBar(QWidget *parent = 0);

    bool autoHide() const;

public slots:
    void setAutoHide(bool autoHide);

signals:
    void autoHideChanged(bool autoHide);

protected:
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(DScrollBar)
    D_DECLARE_PRIVATE(DScrollBar)
    D_PRIVATE_SLOT(void _q_hidden())
    D_PRIVATE_SLOT(void _q_updateOpacity())
};

DWIDGET_END_NAMESPACE

#endif // DSCROLLBAR_H
