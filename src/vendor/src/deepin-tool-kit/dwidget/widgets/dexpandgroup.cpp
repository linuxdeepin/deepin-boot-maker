/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dexpandgroup.h"

DWIDGET_USE_NAMESPACE

DExpandGroup::DExpandGroup(QObject *parent) : QObject(parent)
{

}

QList<DBaseExpand *> DExpandGroup::expands() const
{
    return m_expandMap.values();
}

DBaseExpand *DExpandGroup::checkedExpand() const
{
    if (m_checkedMap.isEmpty())
        return NULL;
    else
        return m_checkedMap.first();
}

DBaseExpand *DExpandGroup::expand(int id) const
{
    return m_expandMap.value(id);
}

void DExpandGroup::addExpand(DBaseExpand *expand, int id)
{
    if (m_expandMap.values().indexOf(expand) == -1){
        if (id == -1){
            int maxId = -1;
            foreach (int tmp, m_expandMap.keys()) {
                maxId = qMax(tmp, maxId);
            }

            id = maxId + 1;
        }
        m_expandMap.insert(id, expand);

        connect(expand, &DBaseExpand::expandChange, this, &DExpandGroup::onExpandChanged);
    }
}

void DExpandGroup::setId(DBaseExpand *expand, int id)
{
    int index = m_expandMap.values().indexOf(expand);
    if (index != -1){
        m_expandMap.remove(m_expandMap.keys().at(index));
        addExpand(expand, id);
    }
}

void DExpandGroup::removeExpand(DBaseExpand *expand)
{
    int index = m_expandMap.values().indexOf(expand);
    m_expandMap.remove(m_expandMap.keys().at(index));
}

int DExpandGroup::checkedId() const
{
    if (m_checkedMap.isEmpty())
        return -1;
    else
        return m_checkedMap.firstKey();
}

int DExpandGroup::id(DBaseExpand *expand) const
{
    return m_expandMap.keys().at(m_expandMap.values().indexOf(expand));
}

void DExpandGroup::onExpandChanged(bool v)
{
    DBaseExpand *dSender = qobject_cast<DBaseExpand *>(sender());
    if (!dSender || dSender == checkedExpand() || !v)
        return;

    m_checkedMap.clear();
    int checkedIndex = m_expandMap.values().indexOf(dSender);
    m_checkedMap.insert(m_expandMap.keys().at(checkedIndex), dSender);

    QList<DBaseExpand *> expandList = m_expandMap.values();
    for (int i = 0; i < expandList.count(); i ++){
        DBaseExpand *target = expandList.at(i);
        if (target != dSender && target->expand())
            target->setExpand(false);
    }
}


