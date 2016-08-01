/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "doption.h"
#include "dwidget_global.h"
#include "dthememanager.h"
#include "private/doption_p.h"

#include <QDebug>
#include <QResizeEvent>

DWIDGET_BEGIN_NAMESPACE

DOptionPrivate::DOptionPrivate(DOption *q) :
    DObjectPrivate(q)
{
}

void DOptionPrivate::init()
{
    D_Q(DOption);

    m_checkedIconLabel = new QLabel;
    m_checkedIconLabel->setMinimumSize(0, 0);
    m_checkedIconLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_optionIcon = new QLabel;
    m_optionIcon->setMinimumSize(0, 0);
    m_optionIcon->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_optionName = new QLabel;
    m_optionName->setObjectName("OptionName");
    m_optionName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_layout = new QHBoxLayout;
    m_layout->addWidget(m_checkedIconLabel);
    m_layout->addWidget(m_optionIcon);
    m_layout->addWidget(m_optionName);
    m_layout->setSpacing(5);
    m_layout->setContentsMargins(0, 0, 0, 0);

    q->setLayout(m_layout);
}

void DOptionPrivate::setIcon(const QString &iconName)
{
    setIcon(QPixmap(iconName));
}

void DOptionPrivate::setIcon(const QPixmap &icon)
{
    m_optionIcon->setPixmap(icon);
}

const QString DOptionPrivate::name() const
{
    return m_optionName->text();
}

const QString DOptionPrivate::value() const
{
    return m_optionValue;
}

bool DOptionPrivate::checked() const
{
    return m_checked;
}

void DOptionPrivate::setChecked(bool checked)
{
    if (m_checked == checked)
        return;
    m_checked = checked;

    D_QC(DOption);

    emit q->checkedChanged(m_checked);
}

const QString DOptionPrivate::checkedIcon() const
{
    return m_checkedIconName;
}

void DOptionPrivate::setCheckedIcon(const QString &icon)
{
    if (icon == m_checkedIconName)
        return;
    m_checkedIconName = icon;
    m_checkedIconLabel->setPixmap(QPixmap(icon));

    D_QC(DOption);

    emit q->checkedIconChanged(icon);
}

void DOptionPrivate::sizeChanged(QResizeEvent *e)
{
    D_Q(DOption);

    emit q->sizeChanged(e->size());
}

DOption::DOption(QWidget *parent) :
    QFrame(parent),
    DObject(*new DOptionPrivate(this))
{
    D_THEME_INIT_WIDGET(DOption, checked)

    D_D(DOption);

    d->init();
}

void DOption::setName(const QString &name)
{
    D_D(DOption);

    d->m_optionName->setText(name);
}

void DOption::setValue(const QString &value)
{
    D_D(DOption);

    d->m_optionValue = value;
}

void DOption::setIcon(const QString &icon)
{
    D_D(DOption);

    d->setIcon(icon);
}

void DOption::setIcon(const QPixmap &icon)
{
    D_D(DOption);

    d->setIcon(icon);
}

const QString DOption::value() const
{
    D_DC(DOption);

    return d->value();
}

const QString DOption::name() const
{
    D_DC(DOption);

    return d->name();
}

bool DOption::checked() const
{
    D_DC(DOption);

    return d->checked();
}

void DOption::setChecked(bool checked)
{
    D_D(DOption);

    d->setChecked(checked);
}

const QString DOption::checkedIcon() const
{
    D_DC(DOption);

    return d->checkedIcon();
}

void DOption::setCheckedIcon(const QString &icon)
{
    D_D(DOption);

    d->setCheckedIcon(icon);
}

QHBoxLayout *DOption::layout()
{
    D_D(DOption);

    return d->m_layout;
}

void DOption::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    D_D(DOption);

    d->sizeChanged(e);
}

DWIDGET_END_NAMESPACE

#include "moc_doption.cpp"
