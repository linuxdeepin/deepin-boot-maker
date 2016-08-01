/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DOPTION_P_H
#define DOPTION_P_H

#include "doption.h"

#include <private/dobject_p.h>

#include <QLabel>
#include <QHBoxLayout>
#include <QListWidgetItem>

DWIDGET_BEGIN_NAMESPACE

class DOptionPrivate : public DObjectPrivate
{
    D_DECLARE_PUBLIC(DOption)

public:
    DOptionPrivate(DOption *q);

    void init();

    void setIcon(const QString& iconName);
    void setIcon(const QPixmap& icon);

    const QString name() const;
    const QString value() const;

    bool checked() const;
    void setChecked(bool checked);

    const QString checkedIcon() const;
    void setCheckedIcon(const QString &icon);

    void sizeChanged(QResizeEvent *e);

public slots:
    void _q_itemStateChanged(QListWidgetItem* item);

public:
    bool m_checked = false;
    QString m_optionValue;
    QString m_checkedIconName;

    QLabel *m_checkedIconLabel;
    QLabel *m_optionIcon;
    QLabel *m_optionName;

    QHBoxLayout *m_layout;
};

DWIDGET_END_NAMESPACE

#endif // DOPTION_P_H

