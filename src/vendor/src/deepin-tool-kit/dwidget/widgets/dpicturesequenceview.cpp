/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dpicturesequenceview.h"
#include "private/dpicturesequenceview_p.h"

#include <QGraphicsPixmapItem>

DWIDGET_BEGIN_NAMESPACE

DPictureSequenceViewPrivate::DPictureSequenceViewPrivate(DPictureSequenceView *q) :
    DObjectPrivate(q)
{

}

void DPictureSequenceViewPrivate::init()
{
    D_Q(DPictureSequenceView);

    m_scene = new QGraphicsScene(q);
    m_refreshTimer = new QTimer(q);
    m_refreshTimer->setInterval(33);
    m_refreshTimer->start();

    q->setScene(m_scene);
    q->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    q->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    q->setFrameShape(QFrame::NoFrame);

    q->connect(m_refreshTimer, &QTimer::timeout, [this] {refreshPicture();});
}

void DPictureSequenceViewPrivate::play()
{
    m_refreshTimer->start();
}

void DPictureSequenceViewPrivate::setPictureSequence(const QStringList &sequence,
                                                     DPictureSequenceView::PaintMode paintMode)
{
    D_QC(DPictureSequenceView);

    for (const QString &pic : sequence) {
        QPixmap pixmap(pic);

        if (paintMode == DPictureSequenceView::AutoScaleMode)
            pixmap = pixmap.scaled(q->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

        addPicture(pixmap);
    }
}

int DPictureSequenceViewPrivate::speed() const
{
    return m_refreshTimer->interval();
}

void DPictureSequenceViewPrivate::setSpeed(int speed)
{
    m_refreshTimer->setInterval(speed);
}

bool DPictureSequenceViewPrivate::singleShot() const
{
    return m_singleShot;
}

void DPictureSequenceViewPrivate::setSingleShot(bool singleShot)
{
    m_singleShot = singleShot;
}

void DPictureSequenceViewPrivate::refreshPicture()
{
    m_pictureList[m_lastItemPos++]->hide();

    if (m_lastItemPos == m_pictureList.count())
    {
        m_lastItemPos = 0;

        if (m_singleShot)
            m_refreshTimer->stop();

        D_QC(DPictureSequenceView);

        emit q->playEnd();
    }

    m_pictureList[m_lastItemPos]->show();
}

void DPictureSequenceViewPrivate::addPicture(const QPixmap &pixmap)
{
    QGraphicsPixmapItem *item = m_scene->addPixmap(pixmap);
    item->hide();
    m_pictureList.append(item);
}

DPictureSequenceView::DPictureSequenceView(QWidget *parent) :
    QGraphicsView(parent),
    DObject(*new DPictureSequenceViewPrivate(this))
{
    D_D(DPictureSequenceView);

    d->init();
}

void DPictureSequenceView::setPictureSequence(const QStringList &sequence)
{
    setPictureSequence(sequence, NormalMode);
}

void DPictureSequenceView::setPictureSequence(const QStringList &sequence, PaintMode paintMode)
{
    D_D(DPictureSequenceView);

    d->setPictureSequence(sequence, paintMode);

    setStyleSheet("background-color:transparent;");
}

void DPictureSequenceView::play()
{
    D_D(DPictureSequenceView);

    d->play();
}

int DPictureSequenceView::speed() const
{
    D_DC(DPictureSequenceView);

    return d->speed();
}

void DPictureSequenceView::setSpeed(int speed)
{
    D_D(DPictureSequenceView);

    d->setSpeed(speed);
}

bool DPictureSequenceView::singleShot() const
{
    D_DC(DPictureSequenceView);

    return d->singleShot();
}

void DPictureSequenceView::setSingleShot(bool singleShot)
{
    D_D(DPictureSequenceView);

    d->setSingleShot(singleShot);
}

DWIDGET_END_NAMESPACE

#include "moc_dpicturesequenceview.cpp"
