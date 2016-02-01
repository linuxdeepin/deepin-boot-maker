/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DIMAGEICON_H
#define DIMAGEICON_H

#include "dserialframemovie.h"
#include "dtips.h"
#include <QWidget>

class DSerialFrameMovie;

class DImageIcon : public DSerialFrameMovie
{
    Q_OBJECT
public:
    enum ImageStatus {
        UnSelected,
        Selected,
        Progress,
    };


    explicit DImageIcon(QWidget *parent = 0);

    void setStatus(ImageStatus status) {
        m_Status = status;
        emit statusChanged(m_Status);
    }

    ImageStatus status() {
        return m_Status;
    }

signals:
    void statusChanged(ImageStatus);

public slots:
    void updateStatus(ImageStatus);

protected:
    ImageStatus         m_Status;
    QStringList         m_BreatheMovieSources;
    QStringList         m_RoateMovieSources;
};

#endif // DIMAGEICON_H
