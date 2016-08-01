/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DFILECHOOSEREDIT_P_H
#define DFILECHOOSEREDIT_P_H

#include "dlineedit_p.h"
#include "dfilechooseredit.h"
#include "dimagebutton.h"

DWIDGET_BEGIN_NAMESPACE

class DFileChooserEditPrivate : DLineEditPrivate
{
    Q_DECLARE_PUBLIC(DFileChooserEdit)

public:
    DFileChooserEditPrivate(DFileChooserEdit *q);

    void init();

public:
    void _q_showFileChooserDialog();

    DFileChooserEdit::DialogDisplayPosition dialogDisplayPosition = DFileChooserEdit::FoloowParentWindow;
};

DWIDGET_END_NAMESPACE

#endif // DFILECHOOSEREDIT_P_H

