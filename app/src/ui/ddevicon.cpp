/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "ddevicon.h"
#include "dserialframemovie.h"
#include "dusbprogressmovie.h"

#include <QStackedLayout>
#include <QHBoxLayout>
#include <QLabel>

DDevIcon::DDevIcon(QWidget *parent) :
    QWidget(parent)
{
    QStackedLayout *m_Layout = new QStackedLayout;
    m_Layout->setStackingMode(QStackedLayout::StackAll);
    int m_breatheListNumber = 23;
    QStringList breatheSourceList;
    for (int i = 1; i < m_breatheListNumber; ++i) {
        breatheSourceList.append(QString(":/ui/images/usb/%1.png").arg(i));
    }
    m_BreatheMovie = new DSerialFrameMovie(breatheSourceList, this);
    m_BreatheMovie->setDTips("<p style='font-weight:normal;'>"+ tr("Select USB drive") + "</p>");
    m_BreatheMovie->showDTips(true);
    m_Layout->addWidget(m_BreatheMovie);
    m_Layout->setAlignment(m_BreatheMovie, Qt::AlignHCenter);

    m_ProgressWidget = new QWidget;
    QHBoxLayout *progressLayout = new QHBoxLayout;
    progressLayout->addSpacing(6);
    m_ProgressMovie = new DUsbProgressMovie;
    progressLayout->addWidget(m_ProgressMovie);
    m_ProgressWidget->setLayout(progressLayout);
    m_Layout->addWidget(m_ProgressWidget);

    m_LightWidget = new QWidget;
    QHBoxLayout *lightLayout = new QHBoxLayout;
    lightLayout->addSpacing(15);
    lightLayout->setContentsMargins(0,0,0,1);
    lightLayout->setSpacing(0);
    QLabel *m_Light = new QLabel;
    m_Light->setPixmap(QPixmap(":/ui/images/usb/light.png"));
    lightLayout->addWidget(m_Light);
    lightLayout->setAlignment(m_Light, Qt::AlignVCenter);
    m_LightWidget->setLayout(lightLayout);
    m_Layout->addWidget(m_LightWidget);
    m_LightWidget->setAttribute(Qt::WA_TransparentForMouseEvents,true);

    this->setLayout(m_Layout);
    setStatus(UnSelected);

    connect(m_BreatheMovie, SIGNAL(clicked()), this, SLOT(emitClicked()));
}

void DDevIcon::emitClicked() {
    emit clicked();
}

DDevIcon::DevStatus DDevIcon::Status() const
{
    return m_Status;
}

void DDevIcon::setStatus(const DevStatus &Status)
{
    m_Status = Status;
    switch (m_Status) {
    case UnSelected:
        m_BreatheMovie->show();
        m_BreatheMovie->start();
        m_BreatheMovie->showDTips(true);
        m_ProgressWidget->hide();
        m_LightWidget->hide();
        break;
    case Selected:
        m_BreatheMovie->show();
        m_BreatheMovie->stop();
        m_BreatheMovie->showDTips(false);
        m_ProgressWidget->hide();
        m_LightWidget->show();
        break;
    case Progress:
        m_BreatheMovie->hide();
        m_BreatheMovie->stop();
        m_BreatheMovie->showDTips(false);
        m_LightWidget->hide();
        m_ProgressWidget->show();
        m_ProgressMovie->start();
        break;
    default:
        break;
    }
}

void DDevIcon::setProgress(int p) {
    if (m_ProgressMovie) {
        m_ProgressMovie->setProgress(p);
    }
}
