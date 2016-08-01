/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DOPTIONLIST_H
#define DOPTIONLIST_H

#include "dwidget_global.h"
#include "dobject.h"
#include "doption.h"

#include <QListWidget>

DWIDGET_BEGIN_NAMESPACE

class DOptionListPrivate;
class LIBDTKWIDGETSHARED_EXPORT DOptionList : public QListWidget, public DObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DOptionList)
    D_DECLARE_PRIVATE(DOptionList)

public:
    explicit DOptionList(QWidget *parent = nullptr);

    void addOption(DOption *option);
    void insertOption(int pos, DOption *option);

    void setCurrentSelected(int pos);
    void setCurrentSelected(const QString &value);

protected:
    D_PRIVATE_SLOT(void _q_currentItemChanged(QListWidgetItem*,QListWidgetItem*))
};

DWIDGET_END_NAMESPACE

#endif // DOPTIONLIST_H
