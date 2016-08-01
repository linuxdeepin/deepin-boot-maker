/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef EXPANDGROUP_H
#define EXPANDGROUP_H

#include <QObject>
#include <QList>
#include <QMap>

#include "dwidget_global.h"
#include "dbaseexpand.h"

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DExpandGroup : public QObject
{
    Q_OBJECT
public:
    explicit DExpandGroup(QObject *parent = 0);

    QList<DBaseExpand *> expands() const;
    DBaseExpand * checkedExpand() const;
    DBaseExpand * expand(int id) const;
    void addExpand(DBaseExpand *expand, int id = -1);
    void setId(DBaseExpand *expand, int id);
    void removeExpand(DBaseExpand *expand);
    int checkedId() const;
    int id(DBaseExpand *expand) const;

private:
    void onExpandChanged(bool v);

private:
    QMap<int, DBaseExpand *> m_expandMap;
    QMap<int, DBaseExpand *> m_checkedMap;
};

DWIDGET_END_NAMESPACE

#endif // EXPANDGROUP_H
