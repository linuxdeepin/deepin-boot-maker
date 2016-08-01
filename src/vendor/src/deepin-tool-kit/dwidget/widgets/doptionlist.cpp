/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/


#include "doptionlist.h"
#include "dwidget_global.h"
#include "dthememanager.h"
#include "private/doptionlist_p.h"

#include <QDebug>

DWIDGET_BEGIN_NAMESPACE

DOptionListPrivate::DOptionListPrivate(DOptionList *q) :
    DObjectPrivate(q)
{

}

void DOptionListPrivate::init()
{
    D_Q(DOptionList);

    q->setSelectionMode(QAbstractItemView::SingleSelection);
    q->connect(q, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), q, SLOT(_q_currentItemChanged(QListWidgetItem*,QListWidgetItem*)));
}

void DOptionListPrivate::addOption(DOption *option)
{
    QListWidgetItem *item = new QListWidgetItem;
    m_optionList.insert(item, option);

    D_Q(DOptionList);

    q->connect(option, &DOption::sizeChanged, [item] (const QSize &size) {item->setSizeHint(size);});

    q->addItem(item);
    q->setItemWidget(item, option);
}

void DOptionListPrivate::insertOption(int pos, DOption *option)
{
    QListWidgetItem *item = new QListWidgetItem;
    m_optionList.insert(item, option);

    D_Q(DOptionList);

    q->connect(option, &DOption::sizeChanged, [item] (const QSize &size) {item->setSizeHint(size);});

    q->insertItem(pos, item);
    q->setItemWidget(item, option);
}

void DOptionListPrivate::setCurrentSelected(int pos)
{
    D_Q(DOptionList);

    DOption *prevItem = m_optionList.value(q->currentItem(), nullptr);

    if (prevItem)
        prevItem->setChecked(false);

    QListWidgetItem *item = q->item(pos);
    q->setCurrentItem(item);

    DOption *currentItem = m_optionList.value(item, nullptr);
    if (currentItem)
        currentItem->setChecked(true);
}

void DOptionListPrivate::setCurrentSelected(const QString &value)
{
    D_Q(DOptionList);

    const QList<DOption *> list = m_optionList.values();

    for (DOption *option : list)
        if (option->value() == value)
            return setCurrentSelected(q->row(m_optionList.key(option)));
}

void DOptionListPrivate::_q_currentItemChanged(QListWidgetItem *current,QListWidgetItem *previous)
{
    DOption *currentItem = m_optionList.value(current, nullptr);
    DOption *previousItem = m_optionList.value(previous, nullptr);

    if (currentItem)
        currentItem->setChecked(true);
    if (previousItem)
        previousItem->setChecked(false);
}

DOptionList::DOptionList(QWidget *parent) :
    QListWidget(parent),
    DObject(*new DOptionListPrivate(this))
{
    D_THEME_INIT_WIDGET(DOptionList);

    D_D(DOptionList);

    d->init();
}

void DOptionList::addOption(DOption *option)
{
    D_D(DOptionList);
    d->addOption(option);
}

void DOptionList::insertOption(int pos, DOption *option)
{
    D_D(DOptionList);
    d->insertOption(pos, option);
}

void DOptionList::setCurrentSelected(int pos)
{
    D_D(DOptionList);

    d->setCurrentSelected(pos);
}

void DOptionList::setCurrentSelected(const QString &value)
{
    D_D(DOptionList);

    d->setCurrentSelected(value);
}

DWIDGET_END_NAMESPACE

#include "moc_doptionlist.cpp"
