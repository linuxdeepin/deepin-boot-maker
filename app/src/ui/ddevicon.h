/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DDEVICON_H
#define DDEVICON_H

#include <QWidget>
#include "dtips.h"

class DSerialFrameMovie;
class DUsbProgressMovie;

class QLabel;

class DDevIcon : public QWidget{
    Q_OBJECT
public:
    enum DevStatus {
        UnSelected,
        Selected,
        Progress,
    };

    explicit DDevIcon(QWidget *parent = 0);

    DevStatus Status() const;
    void setStatus(const DevStatus &Status);

    void setProgress(int p);

signals:
    void clicked();

protected slots:
    void emitClicked();

protected:

    DSerialFrameMovie   *m_BreatheMovie;
    DUsbProgressMovie   *m_ProgressMovie;
    QWidget             *m_ProgressWidget;
    QWidget             *m_LightWidget;
    DevStatus           m_Status;
};

#endif // DDEVICON_H
