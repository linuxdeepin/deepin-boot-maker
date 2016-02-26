/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DUSBPROGRESSMOVIE_H
#define DUSBPROGRESSMOVIE_H

#include <QLabel>
#include <QPixmap>

class QTimer;

class DWaterMovie : public QLabel
{
    Q_OBJECT
public:
    explicit DWaterMovie(QWidget *parent = 0);

    virtual void paintEvent(QPaintEvent *);

    void setProgressImage(const QString& qrl, int begin, int end);

    //0~100
    void setProgress(int p);

    void start();

signals:

public slots:
    void updateProcess();

protected:
    QTimer  *m_timer;
    QImage  *m_Mask;
    QImage  *m_Image;

    QString m_ImagePath;
    int     m_YMin;
    int     m_YMax;

    int     m_XOffset;
    int     m_YOffset;
    int     m_ImageWidth;
    int     m_MaskWidth;
    int     m_MasHeight;
};

class DUsbProgressMovie : public QWidget
{
    Q_OBJECT
public:
    explicit DUsbProgressMovie(QWidget *parent = 0);

    //virtual void paintEvent(QPaintEvent *);
signals:

public slots:
    void start();
    void setProgress(int p);

protected:
    DWaterMovie * m_WaterProgress;
};

#endif // DUSBPROGRESSMOVIE_H
