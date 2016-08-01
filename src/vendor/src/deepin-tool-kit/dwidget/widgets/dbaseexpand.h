/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DBASEEXPAND_H
#define DBASEEXPAND_H

#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QPropertyAnimation>

#include "dwidget_global.h"
#include "dseparatorhorizontal.h"
#include "dconstants.h"

DWIDGET_BEGIN_NAMESPACE

class ContentLoader : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(int height READ height WRITE setFixedHeight)
public:
    explicit ContentLoader(QWidget *parent = 0) : QFrame(parent){}
};

class DBoxWidget;

class LIBDTKWIDGETSHARED_EXPORT DBaseExpand : public QWidget
{
    Q_OBJECT
public:
    explicit DBaseExpand(QWidget *parent = 0);
    ~DBaseExpand();

    void setHeader(QWidget *header);
    void setContent(QWidget *content, Qt::Alignment alignment = Qt::AlignHCenter);
    inline QWidget *getContent() const {return m_content;}
    void setHeaderHeight(int height);
    virtual void setExpand(bool value);
    bool expand() const;
    void setAnimationDuration(int duration);
    void setAnimationEasingCurve(QEasingCurve curve);
    void setSeparatorVisible(bool arg);
    void setExpandedSeparatorVisible(bool arg);

signals:
    void expandChange(bool e);
    void sizeChanged(QSize s);

private:
    QWidget *m_header = NULL;
    QWidget *m_content = NULL;
    DBoxWidget *m_boxWidget = NULL;
    QVBoxLayout *m_headerLayout = NULL;
    QBoxLayout *m_contentLayout = NULL;
    ContentLoader *m_contentLoader = NULL;
    DSeparatorHorizontal * m_hSeparator = NULL;
    DSeparatorHorizontal *m_bottom_separator = NULL;
    QPropertyAnimation *m_animation = NULL;
    bool m_expand = false;
};

DWIDGET_END_NAMESPACE

#endif // DBASEEXPAND_H
