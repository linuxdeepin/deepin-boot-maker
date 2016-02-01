/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DDIGITPROGRESSMOVIE_H
#define DDIGITPROGRESSMOVIE_H

#include <QLabel>

class DDigitProgressMovie : public QWidget
{
    Q_OBJECT
public:
    explicit DDigitProgressMovie(QWidget *parent = 0);

    virtual void paintEvent(QPaintEvent *);

signals:

public slots:

};

#endif // DDIGITPROGRESSMOVIE_H
