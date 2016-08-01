/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dlineedit.h"
#include "dthememanager.h"
#include "private/dlineedit_p.h"

#include <QHBoxLayout>
#include <QDebug>
#include <QResizeEvent>

DWIDGET_BEGIN_NAMESPACE

DLineEdit::DLineEdit(QWidget *parent)
    : QLineEdit(parent),
      DObject(*new DLineEditPrivate(this))
{
    D_THEME_INIT_WIDGET(DLineEdit, alert);

    Q_D(DLineEdit);
    d->init();
}

DLineEdit::DLineEdit(DLineEditPrivate &q, QWidget *parent)
    : QLineEdit(parent),
      DObject(q)
{
    D_THEME_INIT_WIDGET(DLineEdit, alert);

    Q_D(DLineEdit);
    d->init();
}

void DLineEdit::setAlert(bool isAlert)
{
    Q_D(DLineEdit);

    if (isAlert == d->m_isAlert)
        return;

    d->m_isAlert = isAlert;

    emit alertChanged(isAlert);
}

bool DLineEdit::isAlert() const
{
    D_DC(DLineEdit);

    return d->m_isAlert;
}

void DLineEdit::setIconVisible(bool visible)
{
    Q_D(DLineEdit);

    if (visible == d->m_rightIcon->isVisible())
        return;

    d->m_rightIcon->setVisible(visible);
}

bool DLineEdit::iconVisible() const
{
    D_DC(DLineEdit);

    return d->m_rightIcon->isVisible();
}

QString DLineEdit::normalIcon() const
{
    D_DC(DLineEdit);

    return d->m_rightIcon->getNormalPic();
}

void DLineEdit::setNormalIcon(const QString &normalIcon)
{
    Q_D(DLineEdit);

    d->m_rightIcon->setNormalPic(normalIcon);
}

QString DLineEdit::hoverIcon() const
{
    D_DC(DLineEdit);

    return d->m_rightIcon->getHoverPic();
}

void DLineEdit::setHoverIcon(const QString &hoverIcon)
{
    Q_D(DLineEdit);

    d->m_rightIcon->setHoverPic(hoverIcon);
}

QString DLineEdit::pressIcon() const
{
    D_DC(DLineEdit);

    return d->m_rightIcon->getPressPic();
}

void DLineEdit::setPressIcon(const QString &pressIcon)
{
    Q_D(DLineEdit);

    d->m_rightIcon->setPressPic(pressIcon);
}

void DLineEdit::focusInEvent(QFocusEvent *e)
{
    emit focusChanged(true);
    QLineEdit::focusInEvent(e);
}

void DLineEdit::focusOutEvent(QFocusEvent *e)
{
    emit focusChanged(false);
    QLineEdit::focusOutEvent(e);
}

void DLineEdit::resizeEvent(QResizeEvent *e)
{
    QLineEdit::resizeEvent(e);

    emit sizeChanged(e->size());
}

DLineEditPrivate::DLineEditPrivate(DLineEdit *q)
    : DObjectPrivate(q)
{
}

void DLineEditPrivate::init()
{
    Q_Q(DLineEdit);
    m_insideFrame = new QFrame(q);
    m_insideFrame->setObjectName("LineEditInsideFrame");
    m_rightIcon = new DImageButton;
    m_rightIcon->hide();
    m_centeralHLayout = new QHBoxLayout;
    m_centeralHLayout->addStretch();
    m_centeralHLayout->addWidget(m_rightIcon);
    m_centeralHLayout->setSpacing(0);
    m_centeralHLayout->setMargin(0);

    q->setLayout(m_centeralHLayout);
    q->setContextMenuPolicy(Qt::NoContextMenu);

    q->connect(m_rightIcon, &DImageButton::clicked, q, &DLineEdit::iconClicked);
    q->connect(q, SIGNAL(sizeChanged(QSize)), q, SLOT(_q_resizeInsideFrame(QSize)));
}

void DLineEditPrivate::_q_resizeInsideFrame(const QSize &size)
{
    m_insideFrame->setFixedHeight(size.height() - 1);
    m_insideFrame->setFixedWidth(size.width());
}

DWIDGET_END_NAMESPACE

#include "moc_dlineedit.cpp"
