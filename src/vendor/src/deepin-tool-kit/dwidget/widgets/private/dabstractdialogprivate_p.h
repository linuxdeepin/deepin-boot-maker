/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DABSTRACTDIALOG_P_H
#define DABSTRACTDIALOG_P_H

#include "dabstractdialog.h"

#include <private/dobject_p.h>

class QBoxLayout;

DWIDGET_BEGIN_NAMESPACE

class DPlatformWindowHandle;

class DAbstractDialogPrivate : public DObjectPrivate
{
protected:
    DAbstractDialogPrivate(DAbstractDialog *qq);

    QRect getParentGeometry() const;

private:
    void init();

    bool mouseMoved = false;
    /// 记录是否收到过鼠标移动事件.
    bool mousePressed = false;

    QPoint dragPosition;
    QColor backgroundColor;
    QColor borderColor;
    DAbstractDialog::DisplayPostion displayPostion;

    DPlatformWindowHandle *handle = Q_NULLPTR;

    Q_DECLARE_PUBLIC(DAbstractDialog)
};

DWIDGET_END_NAMESPACE

#endif // DABSTRACTDIALOG_P_H

