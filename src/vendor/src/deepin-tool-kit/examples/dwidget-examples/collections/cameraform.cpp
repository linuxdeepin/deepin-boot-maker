/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "cameraform.h"
#include "ui_cameraform.h"

#include <QCamera>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QMediaRecorder>
#include <QMediaMetaData>
#include <QVideoProbe>
#include <QCameraImageCapture>
#include <QDebug>

CameraForm::CameraForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraForm)
{
    ui->setupUi(this);

    camera = new QCamera(QCameraInfo::defaultCamera(), this);
    viewfinder = new DCameraView(ui->widget);
    viewfinder->resize(150,150);
    viewfinder->setAspectRatioMode(Qt::KeepAspectRatioByExpanding);

    viewfinder->move(ui->widget->width()/2-viewfinder->width()/2, ui->widget->height()/2-viewfinder->height()/2);
    viewfinder->setSource(camera);
}

CameraForm::~CameraForm()
{
    camera->stop();
    delete ui;
}

void CameraForm::on_horizontalSlider_valueChanged(int value)
{
    viewfinder->setScale(1+value/50.0);
}

void CameraForm::on_pushButton_clicked()
{
    ui->label->setPixmap(viewfinder->capture());
}

void CameraForm::on_pushButton_2_clicked()
{
    viewfinder->setMirroredHorizontal(!viewfinder->mirroredHorizontal());
}

void CameraForm::on_pushButton_3_clicked()
{
    viewfinder->setRound(!viewfinder->round());
}

void CameraForm::on_pushButton_4_clicked()
{
    if(camera->status() == QCamera::ActiveStatus) {
        camera->stop();
        ui->pushButton_4->setText("start");
    } else {
        camera->start();
        ui->pushButton_4->setText("stop");
    }
}
