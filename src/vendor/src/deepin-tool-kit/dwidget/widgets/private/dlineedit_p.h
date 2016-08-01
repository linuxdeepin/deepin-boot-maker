/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DLINEEDIT_P_H
#define DLINEEDIT_P_H

#include "dlineedit.h"
#include "dimagebutton.h"

#include <private/dobject_p.h>

#include <QHBoxLayout>

DWIDGET_BEGIN_NAMESPACE

class DLineEditPrivate : public DObjectPrivate
{
    Q_DECLARE_PUBLIC(DLineEdit)

public:
    DLineEditPrivate(DLineEdit *q);

    void init();

public slots:
    void _q_resizeInsideFrame(const QSize &size);
    void _q_resizeInputableRegion();

public:
    bool m_isAlert = false;

    QFrame *m_insideFrame;
    DImageButton *m_rightIcon;
    QHBoxLayout *m_centeralHLayout;
};

DWIDGET_END_NAMESPACE

#endif // DLINEEDIT_P_H

