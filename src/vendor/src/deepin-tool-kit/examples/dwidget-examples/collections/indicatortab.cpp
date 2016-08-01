/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "indicatortab.h"

#include "dpicturesequenceview.h"
#include "dlinkbutton.h"

#include <QDebug>
#include <QString>

DWIDGET_USE_NAMESPACE

IndicatorTab::IndicatorTab(QWidget *parent) :
    QLabel(parent)
{
    setStyleSheet("IndicatorTab{background-color: #252627;}");

    //////////////////////////////////////////////////////////////--DLoadingIndicator
    ///
    QLabel *loadingContainer = new QLabel(this);
    QVBoxLayout * loadingContainerLayout = new QVBoxLayout(loadingContainer);
    loadingContainer->setLayout(loadingContainerLayout);
    loadingContainer->setFixedSize(200, 200);

    DLoadingIndicator *loadingIndicator = new DLoadingIndicator(this);
    loadingIndicator->setImageSource(QPixmap(":/images/test.png"));
    loadingIndicator->setLoading(true);
    loadingIndicator->setAniDuration(2000);
    loadingIndicator->setAniEasingCurve(QEasingCurve::OutInCubic);
    QColor color("gray");
    loadingIndicator->setBackgroundColor(color);

    DLoadingIndicator *loadingIndicator2 = new DLoadingIndicator(this);
    QLabel *label = new QLabel;
    label->setPixmap(QPixmap(":/images/reload_normal.png"));
    label->setAttribute(Qt::WA_TranslucentBackground);
    loadingIndicator2->setFixedSize(label->sizeHint());
    loadingIndicator2->setWidgetSource(label);
    loadingIndicator2->setSmooth(true);
    loadingIndicator2->setLoading(true);

    loadingContainerLayout->addWidget(loadingIndicator);
    loadingContainerLayout->addWidget(loadingIndicator2);

    QStringList seq;
    for (int i(1); i != 91; ++i)
        seq.append(QString(":/images/Spinner/Spinner%1.png").arg(i, 2, 10, QChar('0')));

    QStringList seq2;
    for (int i(1); i != 91; ++i)
        seq2.append(QString(":/images/eLoading/eLoading_%1.png").arg(i, 2, 10, QChar('0')));

    DPictureSequenceView *seqView = new DPictureSequenceView(this);
    seqView->setPictureSequence(seq);
    seqView->move(200, 200);
    seqView->setFixedSize(50, 50);
    seqView->setSpeed(20);

    DPictureSequenceView *seqView2 = new DPictureSequenceView(this);
    seqView2->setPictureSequence(seq);
    seqView2->move(200, 250);
    seqView2->setFixedSize(50, 50);
    seqView2->setSpeed(33);

    DPictureSequenceView *seqView3 = new DPictureSequenceView(this);
    seqView3->setPictureSequence(seq);
    seqView3->move(200, 300);
    seqView3->setFixedSize(50, 50);
    seqView3->setSpeed(66);

    DPictureSequenceView *seqView4 = new DPictureSequenceView(this);
    seqView4->setPictureSequence(seq2);
    seqView4->move(250, 200);
    seqView4->setFixedSize(50, 50);
    seqView4->setSpeed(20);

    DPictureSequenceView *seqView5 = new DPictureSequenceView(this);
    seqView5->setPictureSequence(seq2);
    seqView5->move(250, 250);
    seqView5->setFixedSize(50, 50);
    seqView5->setSpeed(33);

    DPictureSequenceView *seqView6 = new DPictureSequenceView(this);
    seqView6->setPictureSequence(seq2);
    seqView6->move(250, 300);
    seqView6->setFixedSize(50, 50);
    seqView6->setSpeed(66);

    DPictureSequenceView *seqViewOnce = new DPictureSequenceView(this);
    seqViewOnce->setPictureSequence(seq);
    seqViewOnce->move(400, 300);
    seqViewOnce->setFixedSize(50, 50);
    seqViewOnce->setSpeed(33);
    seqViewOnce->setSingleShot(true);

    DLinkButton *restart = new DLinkButton("restart", this);
    restart->move(450, 320);

    connect(restart, &DLinkButton::clicked, [seqViewOnce] {
        seqViewOnce->play();
    });
}
