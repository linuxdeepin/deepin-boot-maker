/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dprogress.h"

DProgress::DProgress(QWidget *parent) :
    DSerialFrameMovie(parent)
{
    m_inactiveSources.append(QString(":/ui/images/process/inactive/process-inactive.png"));
    m_emptySources.append(QString(":/ui/images/process/inactive/process-empty.png"));
    int m_activeFrameNumber = 32;
    for (int i = 1; i <= m_activeFrameNumber; ++i) {
        m_activeSources.append(QString(":/ui/images/process/active/arrow_animation_%1.png").arg(i));
    }

    DSerialFrameMovie::setFixedSize(QPixmap(m_inactiveSources[0]).size());
    connect(this, SIGNAL(statusChanged(ProcessStatus)), this, SLOT(updateStatus(ProcessStatus)));
    setStatus(UnProgress);
}

void DProgress::updateStatus(ProcessStatus status) {
    this->showDTips(false);
    m_FrameIndex = 0;
    switch (status) {
    case Empty:
        m_SourceList = m_emptySources;
        this->stop();
        break;
    case UnProgress:
        m_SourceList = m_inactiveSources;
        this->stop();
        break;
    case Progress:
        m_SourceList = m_activeSources;
        DSerialFrameMovie::setFixedSize(QPixmap(m_SourceList[0]).size());
        this->start();
        break;
    default:
        break;
    }
}
