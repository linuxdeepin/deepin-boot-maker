/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dseparatorvertical.h"
#include "dthememanager.h"

DWIDGET_BEGIN_NAMESPACE

DSeparatorVertical::DSeparatorVertical(QWidget *parent) : QWidget(parent)
{
    D_THEME_INIT_WIDGET(DSeparatorVertical);

    this->setFixedWidth(2);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    m_leftRec = new QWidget(this);
    m_leftRec->setObjectName("DSeparatorLeftRec");
    m_rightRec = new QWidget(this);
    m_rightRec->setObjectName("DSeparatorRightRec");

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    mainLayout->addStretch();
    mainLayout->addWidget(m_leftRec);
    mainLayout->addWidget(m_rightRec);
    mainLayout->addStretch();

    this->setLayout(mainLayout);
}

DWIDGET_END_NAMESPACE
