/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DWDIGET_P_H
#define DWDIGET_P_H

#include "private/dobject_p.h"
#include "dwidget.h"
#include "../../platforms/x11/xutil.h"

class QVBoxLayout;

class QGraphicsDropShadowEffect;

class DX11Widget;

DWIDGET_BEGIN_NAMESPACE

class DX11WidgetPrivate: public DObjectPrivate
{
    D_DECLARE_PUBLIC(DX11Widget)
public:
    explicit DX11WidgetPrivate(DX11Widget *q);

    void init();

    QSize externSize(const QSize &size) const;
    QMargins externMargins() const;
    int externWidth() const;
    void updateContentsMargins();

    void _q_onTitleBarMousePressed(Qt::MouseButtons buttons) const;

    bool leftPressed;
    bool resizable;

    int                 m_ShadowWidth;
    int                 m_NormalShadowWidth;
    int                 m_Radius;
    int                 m_Border;
    int                 m_ResizeHandleWidth;
    bool                m_MousePressed;
    QPoint              m_LastMousePos;
    Qt::WindowFlags     dwindowFlags;
    DWidget::DecorationFlags decorationFlags;
    QColor              m_backgroundColor;
    QColor              shadowColor;
    QPoint              shadowOffset;

    QPixmap             m_Background;
    QPixmap             shadowPixmap;
    QWidget             *windowWidget     = nullptr;
    QVBoxLayout         *rootLayout     = nullptr;
    DTitlebar           *titlebar       = nullptr;
    QWidget             *contentWidget  = nullptr;

    XUtils::CornerEdge resizingCornerEdge = XUtils::CornerEdge::kInvalid;
};


class DWidgetPrivate: public DX11WidgetPrivate
{
    D_DECLARE_PUBLIC(DWidget)

public:
    explicit DWidgetPrivate(DWidget *q);

    bool leftPressed;
    bool resizable;
};

DWIDGET_END_NAMESPACE

#endif // DWDIGET_P_H
