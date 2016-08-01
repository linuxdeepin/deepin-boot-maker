/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DSPINBOX_P_H
#define DSPINBOX_P_H

#include "dspinbox.h"

#include <private/dobject_p.h>

class QFrame;
class QLabel;

DWIDGET_BEGIN_NAMESPACE

class DImageButton;
class DSpinBoxPrivate : public DObjectPrivate
{
    DSpinBoxPrivate(DSpinBox *parent);

    void init();

    void _q_resizeInsideFrame(const QSize &size);

    QFrame *insideFrame;
    bool alert = false;
    int defaultValue = 0;

    D_DECLARE_PUBLIC(DSpinBox)
};

class DDoubleSpinBoxPrivate : public DObjectPrivate
{
    DDoubleSpinBoxPrivate(DDoubleSpinBox *parent);

    void init();

    void _q_resizeInsideFrame(const QSize &size);

    QFrame *insideFrame;
    bool alert = false;
    double defaultValue = 0;

    D_DECLARE_PUBLIC(DDoubleSpinBox)
};

DWIDGET_END_NAMESPACE

#endif // DSPINBOX_P_H

