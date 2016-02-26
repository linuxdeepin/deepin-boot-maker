/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DTIPS_H
#define DTIPS_H

#include <QLabel>

class DTips : public QLabel
{
    Q_OBJECT
public:
    explicit DTips(QWidget *parent = 0);

    void paintEvent(QPaintEvent *);
signals:
    void poped();

public slots:
    void pop();
    void pack();

protected slots:
    void popDone();

protected:
    bool    m_active;
    QWidget *m_AttachWidget;
};

#endif // DTIPS_H
