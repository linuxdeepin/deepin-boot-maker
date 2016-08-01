/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dbaseexpand.h"
#include "dthememanager.h"
#include "dboxwidget.h"

#include <QResizeEvent>

DWIDGET_BEGIN_NAMESPACE

DBaseExpand::DBaseExpand(QWidget *parent) : QWidget(parent)
{
    D_THEME_INIT_WIDGET(DBaseExpand);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    m_headerLayout = new QVBoxLayout();
    m_headerLayout->setContentsMargins(0, 0, 0, 0);
    m_headerLayout->setAlignment(Qt::AlignCenter);

    m_hSeparator = new DSeparatorHorizontal();
    m_bottom_separator = new DSeparatorHorizontal;
    m_bottom_separator->hide();

    connect(this, &DBaseExpand::expandChange, m_bottom_separator, &DSeparatorHorizontal::setVisible);

    m_contentLoader = new ContentLoader();
    m_contentLoader->setFixedHeight(0); // default to not expanded.

    m_boxWidget = new DVBoxWidget;
    m_contentLayout = m_boxWidget->layout();

    QVBoxLayout *layout_contentLoader = new QVBoxLayout(m_contentLoader);

    layout_contentLoader->setMargin(0);
    layout_contentLoader->setSpacing(0);
    layout_contentLoader->addWidget(m_boxWidget);
    layout_contentLoader->addStretch();

    m_animation = new QPropertyAnimation(m_contentLoader, "height");
    m_animation->setDuration(200);
    m_animation->setEasingCurve(QEasingCurve::InSine);
    connect(m_animation, &QPropertyAnimation::valueChanged, this, [this] {
        setFixedHeight(sizeHint().height());
    });

    mainLayout->addLayout(m_headerLayout);
    mainLayout->addWidget(m_hSeparator);
    mainLayout->addWidget(m_contentLoader);
    mainLayout->addWidget(m_bottom_separator);

    setLayout(mainLayout);

    connect(m_boxWidget, &DBoxWidget::sizeChanged, this, [this] {
        if (m_expand)
        {
            int endHeight = 0;
            endHeight = m_boxWidget->height();

            m_animation->setStartValue(m_contentLoader->height());
            m_animation->setEndValue(endHeight);
            m_animation->stop();
            m_animation->start();
        }
    });
}

DBaseExpand::~DBaseExpand()
{
    if (m_headerLayout) {
        m_headerLayout->deleteLater();
    }
    if (m_contentLayout) {
        m_contentLayout->deleteLater();
    }
    if (m_contentLoader) {
        m_contentLoader->deleteLater();
    }
    if (m_animation) {
        m_animation->deleteLater();
    }
}

void DBaseExpand::setHeader(QWidget *header)
{
    if (!header) {
        return;
    }

    QLayoutItem *child;
    while ((child = m_headerLayout->takeAt(0)) != 0) {
        delete child;
    }

    m_headerLayout->addWidget(header);
    m_header = header;
}

void DBaseExpand::setContent(QWidget *content, Qt::Alignment alignment)
{
    if (!content) {
        return;
    }

    QLayoutItem *child;
    while ((child = m_contentLayout->takeAt(0)) != 0) {
        delete child;
    }

    m_contentLayout->addWidget(content, 0, alignment);
    m_contentLayout->addStretch(1);
    m_content = content;
}

void DBaseExpand::setHeaderHeight(int height)
{
    if (m_header) {
        m_header->setFixedHeight(height);
    }
}

void DBaseExpand::setExpand(bool value)
{
    if (m_expand == value) {
        return;
    }

    m_expand = value;
    emit expandChange(value);

    if (value) {
        m_animation->setStartValue(0);
        m_animation->setEndValue(m_boxWidget->height());
    } else {
        m_animation->setStartValue(m_boxWidget->height());
        m_animation->setEndValue(0);
    }

    m_animation->stop();
    m_animation->start();
}

bool DBaseExpand::expand() const
{
    return m_expand;
}

void DBaseExpand::setAnimationDuration(int duration)
{
    m_animation->setDuration(duration);
}

void DBaseExpand::setAnimationEasingCurve(QEasingCurve curve)
{
    m_animation->setEasingCurve(curve);
}

void DBaseExpand::setSeparatorVisible(bool arg)
{
    m_hSeparator->setVisible(arg);
}

void DBaseExpand::setExpandedSeparatorVisible(bool arg)
{
    if (arg) {
        connect(this, &DBaseExpand::expandChange, m_bottom_separator, &DSeparatorHorizontal::setVisible);
        m_bottom_separator->show();
    } else {
        disconnect(this, &DBaseExpand::expandChange, m_bottom_separator, &DSeparatorHorizontal::setVisible);
        m_bottom_separator->hide();
    }
}


DWIDGET_END_NAMESPACE
