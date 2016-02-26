/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dimageicon.h"

#include "dserialframemovie.h"
#include "drotemovie.h"

#include <QBoxLayout>

DImageIcon::DImageIcon(QWidget *parent) :
    DSerialFrameMovie(parent)
{
    int m_BreatheFrameNumber = 23;
    for (int i = 1; i < m_BreatheFrameNumber; ++i) {
        m_BreatheMovieSources.append(QString(":/ui/images/iso/breathe/%1.png").arg(i));
    }

    int m_RoteFrameNumber = 72;
    for (int i = 1; i < m_RoteFrameNumber; ++i) {
        m_RoateMovieSources.append(QString(":/ui/images/iso/roate/%1.png").arg(i));
    }

    //this->setFixedSize(QPixmap(m_BreatheMovieSources[0]).size());
    DSerialFrameMovie::setFixedSize(QPixmap(m_BreatheMovieSources[0]).size());
    connect(this, SIGNAL(statusChanged(ImageStatus)), this, SLOT(updateStatus(ImageStatus)));
    setStatus(UnSelected);
}

void DImageIcon::updateStatus(ImageStatus status) {
    switch (status) {
    case UnSelected:
        m_FrameIndex = 0;
        m_SourceList = m_BreatheMovieSources;
        this->showDTips(true);
        this->start();
        break;
    case Selected:
        m_FrameIndex = 0;
        m_SourceList = m_BreatheMovieSources;
        this->showDTips(false);
        this->stop();
        break;
    case Progress:
        m_FrameIndex = 0;
        m_SourceList = m_RoateMovieSources;
        this->showDTips(false);
        this->start();
        break;
    default:
        break;
    }
}
