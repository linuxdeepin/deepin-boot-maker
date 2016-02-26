/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dprogressframe.h"
#include "drotemovie.h"
#include "dusblist.h"
#include "dtips.h"
#include "dpushbutton.h"
#include "dserialframemovie.h"
#include "ddigitprogressmovie.h"
#include "dusbprogressmovie.h"
#include "dimageicon.h"
#include "ddevicon.h"
#include "dprogress.h"

#include <XSys>
#include "../utils.h"

#include <QDebug>
#include <QLabel>
#include <QBoxLayout>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QStackedLayout>
#include <QFileDialog>
#include <QMovie>


QString TopShadowShow =
        "QWidget{"
            "background-color : qlineargradient(spread:reflect, x1:0, y1:0, x2:0, y2:1,"
            "stop:0 rgba(35, 38, 41, 255),"
            "stop:0.05 rgba(35, 38, 41, 0),"
            "stop:0.5 rgba(35, 38, 41, 0),"
            "stop:0.95 rgba(35, 38, 41, 0),"
            "stop:1 rgba(35, 38, 41, 255));"
        "}";


QString s_LabelFormat ="<p style='color:#585959; font-size:12px;'>%1</p>";

DProgressFrame::DProgressFrame(QWidget *parent) :
    QStackedWidget(parent)
{
    int m_height = 240;
    this->setFixedHeight(m_height);
    m_FirstWidget = new QWidget();

    m_FirstLayout = new QVBoxLayout();
#ifdef Q_OS_MAC
   m_FirstLayout->setSpacing(6);
#endif
    m_FirstLayout->addSpacing(12);
    m_isoName = new QLabel(s_LabelFormat.arg(" "));
    m_FirstLayout->addWidget(m_isoName);
    m_FirstLayout->setAlignment(m_isoName, Qt::AlignCenter);

    m_IsoLabel = new DImageIcon(this);
    m_IsoLabel->setDTips("<p style='font-weight:normal;'>"+tr("Select disk image")+"</p>");
    m_FirstLayout->addWidget(m_IsoLabel);
    m_FirstLayout->setAlignment(m_IsoLabel, Qt::AlignCenter);
    connect(m_IsoLabel, SIGNAL(clicked()), this, SLOT(selectISO()));

    m_ProcessLabel =new DProgress(this);
    m_ProcessLabel->setStatus(DProgress::UnProgress);

    m_FirstLayout->addSpacing(8);
#ifdef Q_OS_MAC
   // m_FirstLayout->addSpacing(2);
#endif
    m_FirstLayout->addWidget(m_ProcessLabel);
    m_FirstLayout->setAlignment(m_ProcessLabel, Qt::AlignCenter);

    m_UsbLabel = new DDevIcon(this);
    m_FirstLayout->addSpacing(2);
    m_FirstLayout->addWidget(m_UsbLabel);
    m_FirstLayout->setAlignment(m_UsbLabel, Qt::AlignCenter);
    connect(m_UsbLabel, SIGNAL(clicked()), this, SLOT(switchShowStatus()));

    m_usbName = new QLabel(s_LabelFormat.arg(" "));
    m_FirstLayout->addWidget(m_usbName);
    m_FirstLayout->setAlignment(m_usbName, Qt::AlignCenter);

    m_FirstWidget->setLayout(m_FirstLayout);
    this->addWidget(m_FirstWidget);

    m_SecondWidget = new QWidget();
    m_SecondWidget->setFixedSize(220, 130);
    QVBoxLayout *secondLayout = new QVBoxLayout();
    m_UsbList =new DUsbList();
    m_FirstLayout->addStretch();
    secondLayout->addWidget(m_UsbList);
    secondLayout->setAlignment(m_UsbList, Qt::AlignCenter);
    connect(m_UsbList, SIGNAL(itemClick(QString)), this, SLOT(finishSelectDev(QString)));
    connect(m_UsbList, SIGNAL(selectDev(QString)), this, SLOT(usbDevSelected(QString)));

    m_SecondWidget->setLayout(secondLayout);

    this->addWidget(m_SecondWidget);

    m_TopShadow = new QWidget();
    m_TopShadow->setStyleSheet(TopShadowShow);
    m_TopShadow->setFixedHeight(m_height);
    this->addWidget(m_TopShadow);
    m_TopShadow->hide();
    m_TopShadow->raise();
    m_TopShadow->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    m_Active = false;
    m_ShowStatus = ShowFirst;
    m_Speed = 500;
}

void DProgressFrame::finishSelectDev(const QString &) {
    slideProcess();
}

void DProgressFrame::usbDevSelected(const QString & dev) {
    if (dev.isEmpty()) {
        m_usbName->setText(s_LabelFormat.arg(" "));
        m_UsbLabel->setStatus(DDevIcon::UnSelected);
        m_FirstWidget->repaint();
        emit selectEmptyUsb(false);
    } else {
        m_UsbLabel->setStatus(DDevIcon::Selected);
        emit selectEmptyUsb(true);
    }
    setUsbDev(dev);
}

void DProgressFrame::switchProgress() {
    m_isoName->setText(s_LabelFormat.arg(" "));
    m_usbName->setText(s_LabelFormat.arg(" "));
    m_UsbLabel->setStatus(DDevIcon::Progress);
    m_IsoLabel->setStatus(DImageIcon::Progress);
    m_ProcessLabel->setStatus(DProgress::Progress);
    m_IsoLabel->setDisabled(true);
#ifdef Q_OS_MAC
    m_isoName->hide();
    m_usbName->hide();
#endif
    m_FirstWidget->repaint();
}

void DProgressFrame::switchShowStatus() {
    switch (m_ShowStatus){
    case ShowFirst:
        this->slideUsbSeclect();
        break;
    case ShowSecond:
        this->slideProcess();
        break;
    default:
        break;
    }
}

void DProgressFrame::slideUsbSeclect() {
    if (m_Active) {
        return;
    }
    else {
        m_Active=true;
    }
    m_usbName->setText(s_LabelFormat.arg(" "));
    this->layout()->setEnabled(false);
    m_TopShadow->show();
    m_ProcessLabel->setStatus(DProgress::Empty);
    emit changedUsbSeclet();

    int offsetx=frameRect().width(); //inherited from mother
    int offsety=frameRect().height();//inherited from mother
    m_SecondWidget->setGeometry(0, 0, offsetx, offsety);
    offsetx=0;
    //offsety=offsety;

    //re-position the next widget outside/aside of the display area
    QPoint pnext=m_SecondWidget->pos();
    QPoint pnow=m_FirstWidget->pos();
    m_FirstWidget->move(pnow.x(), pnow.y()- offsety + 64+ 36);
    m_SecondWidget->move(pnext.x(), pnext.y() + 64+ 36);
    //make it visible/show
    m_SecondWidget->show();
    m_SecondWidget->raise();
    m_TopShadow->raise();
    //animate both, the now and next widget to the side, using movie framework
    QPropertyAnimation *animnow = new QPropertyAnimation(m_FirstWidget, "pos");
    animnow->setDuration(m_Speed);
    animnow->setEasingCurve(QEasingCurve::OutBack);
    animnow->setStartValue(QPoint(pnow.x(), pnow.y()));
    animnow->setEndValue(QPoint(offsetx+pnow.x(), -offsety+pnow.y() + 64 + 36));

    QPropertyAnimation *animnext = new QPropertyAnimation(m_SecondWidget, "pos");
    animnext->setDuration(m_Speed);
    animnext->setEasingCurve(QEasingCurve::OutBack);
    animnext->setStartValue(QPoint(pnext.x(), offsety+pnext.y()));
    animnext->setEndValue(QPoint(pnext.x(), pnext.y() + 64+ 36));

    m_AnimGroup = new QParallelAnimationGroup;
    m_AnimGroup->addAnimation(animnow);
    m_AnimGroup->addAnimation(animnext);

    connect(m_AnimGroup, SIGNAL(finished()),this,SLOT(slideUsbDone()));

    m_Active=true;
    m_AnimGroup->start();
}

void DProgressFrame::slideUsbDone() {
    m_TopShadow->hide();
    m_ShowStatus = ShowSecond;
    m_Active = false;
}

void DProgressFrame::toggleShadow(){

}

void DProgressFrame::slideProcess() {
    if (m_Active) {
        return;
    }
    else {
        m_Active=true;
    }
    this->layout()->setEnabled(true);
    m_TopShadow->show();
    m_ProcessLabel->setStatus(DProgress::UnProgress);
    emit changedProgress();

    int offsetx=frameRect().width(); //inherited from mother
    int offsety=frameRect().height();//inherited from mother

    m_FirstWidget->setGeometry ( 0,  64 - offsety, offsetx, offsety );
    offsetx=0;
    //offsety=offsety;

    //re-position the next widget outside/aside of the display area
    QPoint pnext=m_FirstWidget->pos();
    QPoint pnow=m_SecondWidget->pos();
    //make it visible/show
    m_FirstWidget->show();
    m_FirstWidget->raise();
    m_TopShadow->raise();
    //animate both, the now and next widget to the side, using movie framework
    QPropertyAnimation *animnext = new QPropertyAnimation(m_FirstWidget, "pos");
    animnext->setDuration(m_Speed);
    animnext->setEasingCurve(QEasingCurve::OutBack);
    animnext->setStartValue(QPoint(pnext.x(), pnext.y()));
    animnext->setEndValue(QPoint(pnext.x(), pnext.y() + offsety - 64));

    QPropertyAnimation *animnow = new QPropertyAnimation(m_SecondWidget, "pos");
    animnow->setDuration(m_Speed);
    animnow->setEasingCurve(QEasingCurve::OutBack);
    animnow->setStartValue(QPoint(pnow.x(), pnow.y()));
    animnow->setEndValue(QPoint(pnow.x(), pnow.y() + offsety - 64));

    m_AnimGroup = new QParallelAnimationGroup;
    m_AnimGroup->addAnimation(animnow);
    m_AnimGroup->addAnimation(animnext);

    connect(m_AnimGroup, SIGNAL(finished()),this,SLOT(slideProgressDone()));

    m_Active=true;
    m_AnimGroup->start();
}

#include <QThread>

void DProgressFrame::showUsbName() {
    QString showText = Utils::UsbShowText(m_UsbDev);

    QPoint origin = m_usbName->pos();
    int cx = origin.x() + m_usbName->width()/2;
    int cy = origin.y() - m_usbName->height()/2;
    m_usbName->setText(s_LabelFormat.arg(showText));
    m_usbName->adjustSize();
    QPoint end = QPoint(cx -  m_usbName->width()/2, cy+m_usbName->height()/2);
    QPoint start = QPoint(end.x(), end.y()-12);
    QPropertyAnimation *animnow = new QPropertyAnimation(m_usbName, "pos");
    animnow->setDuration(500);
    animnow->setEasingCurve(QEasingCurve::Linear);
    animnow->setStartValue(start);
    animnow->setEndValue(end);
    QParallelAnimationGroup *animGroup = new QParallelAnimationGroup;
    animGroup->addAnimation(animnow);
    connect(animGroup, SIGNAL(finished()),this,SLOT(showUsbDone()));
    animGroup->start();

    disconnect(m_UsbLabel, SIGNAL(clicked()), this, SLOT(switchShowStatus()));
}

void DProgressFrame::showUsbDone(){
    connect(m_UsbLabel, SIGNAL(clicked()), this, SLOT(switchShowStatus()));
}


void DProgressFrame::slideProgressDone(){
    m_TopShadow->hide();
    m_ShowStatus = ShowFirst;
    m_Active = false;
    showUsbName();
}

void DProgressFrame::selectISO(){
    QFileDialog fileDlg(this);
    fileDlg.setViewMode(QFileDialog::Detail);
    fileDlg.setFileMode(QFileDialog::ExistingFile);
    fileDlg.setNameFilter("ISO (*.iso);;");
    fileDlg.setLabelText(QFileDialog::Reject,  tr("Cancel"));
    if (QFileDialog::Accepted == fileDlg.exec()) {
        QString text = fileDlg.selectedFiles().first();
        setIsoFile(text);
        emit isoFileSelected(text);
        m_IsoLabel->setStatus(DImageIcon::Selected);
        connect(m_UsbLabel, SIGNAL(clicked()), this, SLOT(switchShowStatus()));
        QFileInfo iso(text);
        m_isoName->setText(s_LabelFormat.arg(iso.fileName().right(30)));

        QPoint origin = m_isoName->pos();
        int cx = origin.x() + m_isoName->width()/2;
        int cy = origin.y() - m_isoName->height()/2;
        m_isoName->adjustSize();

        QPoint end = QPoint(cx -  m_isoName->width()/2, cy+m_isoName->height()/2);
        QPoint start = QPoint(end.x(), end.y()+12);
        QPropertyAnimation *animnow = new QPropertyAnimation(m_isoName, "pos");
        animnow->setDuration(500);
        animnow->setEasingCurve(QEasingCurve::Linear);
        animnow->setStartValue(start);
        animnow->setEndValue(end);
        QParallelAnimationGroup *animGroup = new QParallelAnimationGroup;
        animGroup->addAnimation(animnow);
        animGroup->start();
    }
}

void DProgressFrame::refreshUsbDrivers(const QStringList& list) {
    m_UsbList->refreshDriverList(list);
}

QString DProgressFrame::usbDev() const
{
    return m_UsbDev;
}

void DProgressFrame::setUsbDev(const QString &usbDev)
{
    m_UsbDev = usbDev;
}

QString DProgressFrame::isoFile() const
{
    return m_IsoFile;
}

void DProgressFrame::setIsoFile(const QString &isoFile)
{
    m_IsoFile = isoFile;
}

void DProgressFrame::setProgress(int p) {
    m_UsbLabel->setProgress(p);
}
