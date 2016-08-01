/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DIPV4LINEEDIT_P_H
#define DIPV4LINEEDIT_P_H

#include "dlineedit_p.h"
#include "dipv4lineedit.h"

DWIDGET_BEGIN_NAMESPACE

class DIpv4LineEditPrivate : public DLineEditPrivate
{
    DIpv4LineEditPrivate(DIpv4LineEdit *parent);

    void init();

    QLabel *getLabel();
    QLineEdit *getEdit();

    void setFocus(bool focus);

    void _q_updateLineEditText();
    void _q_setIpLineEditText(const QString &text);

    QList<QLineEdit*> editList;
    QWidget *editMainWidget;

    bool focus = false;

    D_DECLARE_PUBLIC(DIpv4LineEdit)
};

DWIDGET_END_NAMESPACE

#endif // DIPV4LINEEDIT_P_H

