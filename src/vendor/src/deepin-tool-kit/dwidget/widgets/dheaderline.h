/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DHEADERLINE_H
#define DHEADERLINE_H

#include <QWidget>
#include <QLabel>

#include "dwidget_global.h"
#include "dbaseline.h"

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DHeaderLine : public DBaseLine
{
    Q_OBJECT
public:
    explicit DHeaderLine(QWidget *parent = 0);
    void setTitle(const QString &title);
    void setContent(QWidget *content);

    QString title() const;

private:
    void setLeftContent(QWidget *content);
    void setRightContent(QWidget *content);

private:
    QLabel *m_titleLabel = NULL;
};

DWIDGET_END_NAMESPACE

#endif // DHEADERLINE_H
