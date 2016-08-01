/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DOPTIONLIST_P_H
#define DOPTIONLIST_P_H

#include "doptionlist.h"
#include "doption.h"

#include <private/dobject_p.h>

#include <QList>
#include <QListWidgetItem>

DWIDGET_BEGIN_NAMESPACE

class DOptionListPrivate : public DObjectPrivate
{
    D_DECLARE_PUBLIC(DOptionList)

public:
    DOptionListPrivate(DOptionList *q);

    void init();
    void addOption(DOption *option);
    void insertOption(int pos, DOption *option);
    void setCurrentSelected(int pos);
    void setCurrentSelected(const QString &value);

public slots:
    void _q_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

protected:
    QMap<QListWidgetItem *, DOption *> m_optionList;
};

DWIDGET_END_NAMESPACE

#endif // DOPTIONLIST_P_H
