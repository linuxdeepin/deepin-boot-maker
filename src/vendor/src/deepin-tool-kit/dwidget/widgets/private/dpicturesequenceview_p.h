/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DPICTURESEQUENCEVIEW_P_H
#define DPICTURESEQUENCEVIEW_P_H

#include "dpicturesequenceview.h"

#include <private/dobject_p.h>

#include <QList>
#include <QGraphicsScene>
#include <QTimer>

DWIDGET_BEGIN_NAMESPACE

class DPictureSequenceViewPrivate : public DObjectPrivate
{
    D_DECLARE_PUBLIC(DPictureSequenceView)

public:
    DPictureSequenceViewPrivate(DPictureSequenceView *q);

    void init();

    void play();
    void setPictureSequence(const QStringList &sequence, DPictureSequenceView::PaintMode paintMode);

    int speed() const;
    void setSpeed(int speed);

    bool singleShot() const;
    void setSingleShot(bool singleShot);

public slots:
    void refreshPicture();

protected:
    void addPicture(const QPixmap &pixmap);

public:
    int m_lastItemPos = 0;
    int m_speed = 0;
    bool m_singleShot = false;

    QGraphicsScene *m_scene;
    QTimer *m_refreshTimer;
    QList<QGraphicsPixmapItem *> m_pictureList;
};

DWIDGET_END_NAMESPACE

#endif // DPICTURESEQUENCEVIEW_P_H

