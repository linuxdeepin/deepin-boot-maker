/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dseparatorhorizontal.h"
#include "dthememanager.h"

DWIDGET_BEGIN_NAMESPACE

DSeparatorHorizontal::DSeparatorHorizontal(QWidget *parent) : QWidget(parent)
{
    D_THEME_INIT_WIDGET(DSeparatorHorizontal);

    this->setFixedHeight(2);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_topRec = new QWidget(this);
    m_topRec->setObjectName("DSeparatorTopRec");
    m_bottomRec = new QWidget(this);
    m_bottomRec->setObjectName("DSeparatorBottomRec");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    mainLayout->addStretch();
    mainLayout->addWidget(m_topRec);
    mainLayout->addWidget(m_bottomRec);
    mainLayout->addStretch();

    this->setLayout(mainLayout);
}

DWIDGET_END_NAMESPACE
