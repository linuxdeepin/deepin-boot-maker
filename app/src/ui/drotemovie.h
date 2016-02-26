/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef QROTEANIMATION_H
#define QROTEANIMATION_H

#include <QWidget>
#include <QPixmap>

class QTimer;

class DRoteMovie : public QWidget
{
    Q_OBJECT
public:
    explicit DRoteMovie(const QPixmap &pixmap, QWidget *parent = 0);

    void paintEvent(QPaintEvent * /*event*/);

    void start();

    void stop();

    void setPixmap(const QPixmap &pixmap);

signals:

public slots:
    void refresh();

private:
    QTimer  *timer_;
    QPixmap pixmap_;
    qreal   degrees_;
};

#endif // QROTEANIMATION_H
