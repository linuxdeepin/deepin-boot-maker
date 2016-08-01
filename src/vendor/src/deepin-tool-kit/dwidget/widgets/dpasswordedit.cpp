/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dpasswordedit.h"
#include "dthememanager.h"
#include "private/dpasswordedit_p.h"

#include <QDebug>

DWIDGET_BEGIN_NAMESPACE

DPasswordEdit::DPasswordEdit(QWidget *parent)
    : DLineEdit(*new DPasswordEditPrivate(this), parent)
{
    D_THEME_INIT_WIDGET(DPasswordEdit);
    D_D(DPasswordEdit);

    d->init();
}

bool DPasswordEdit::isEchoMode() const
{
    return echoMode() == Normal;
}

void DPasswordEdit::setEchoMode(QLineEdit::EchoMode mode)
{
    QLineEdit::setEchoMode(mode);

    setStyleSheet(styleSheet());
}

DPasswordEditPrivate::DPasswordEditPrivate(DPasswordEdit *q)
    : DLineEditPrivate(q)
{

}

void DPasswordEditPrivate::init()
{
    D_Q(DPasswordEdit);

    q->setEchoMode(q->Password);
    q->setTextMargins(0, 0, 16, 0);
    q->setIconVisible(true);

    q->connect(q, SIGNAL(iconClicked()), q, SLOT(_q_toggleEchoMode()));
}

void DPasswordEditPrivate::_q_toggleEchoMode()
{
    D_Q(DPasswordEdit);

    if (q->isEchoMode())
        q->setEchoMode(q->Password);
    else
        q->setEchoMode(q->Normal);
}

DWIDGET_END_NAMESPACE

#include "moc_dpasswordedit.cpp"

