/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DARROWLINEEXPAND_H
#define DARROWLINEEXPAND_H

#include <QWidget>

#include "dwidget_global.h"
#include "dbaseexpand.h"
#include "darrowbutton.h"
#include "dbaseline.h"
#include "dheaderline.h"

DWIDGET_BEGIN_NAMESPACE

class ArrowHeaderLine : public DHeaderLine
{
    Q_OBJECT
public:
    ArrowHeaderLine(QWidget *parent = 0);
    void setExpand(bool value);

signals:
    void mousePress();

protected:
    void mousePressEvent(QMouseEvent *);

private:
    void reverseArrowDirection();
    DArrowButton *m_arrowButton = NULL;
};

class LIBDTKWIDGETSHARED_EXPORT DArrowLineExpand : public DBaseExpand
{
    Q_OBJECT
public:
    explicit DArrowLineExpand(QWidget *parent = 0);
    void setTitle(const QString &title);
    void setExpand(bool value);
    DBaseLine *headerLine();

private:
    void setHeader(QWidget *header);
    void resizeEvent(QResizeEvent *e);

private:
    ArrowHeaderLine *m_headerLine = NULL;
};

DWIDGET_END_NAMESPACE

#endif // DARROWLINEEXPAND_H
