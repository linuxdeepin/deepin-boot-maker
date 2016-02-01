/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DPROGRESS_H
#define DPROGRESS_H

#include <QWidget>
#include "dserialframemovie.h"

class DProgress : public DSerialFrameMovie
{
    Q_OBJECT
public:
    enum ProcessStatus {
        Empty,
        UnProgress,
        Progress,
    };

    explicit DProgress(QWidget *parent = 0);

    void setStatus(ProcessStatus status) {
        m_Status = status;
        emit statusChanged(m_Status);
    }

    ProcessStatus status() {
        return m_Status;
    }

signals:
    void statusChanged(ProcessStatus);

public slots:
    void updateStatus(ProcessStatus);

protected:
    ProcessStatus         m_Status;
    QStringList           m_emptySources;
    QStringList           m_inactiveSources;
    QStringList           m_activeSources;

};

#endif // DPROGRESS_H
