/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef CAMERAFORM_H
#define CAMERAFORM_H

#include <QWidget>
#include <QVideoFrame>
#include "dcameraview.h"

namespace Ui {
class CameraForm;
}

class QCamera;
class QCameraViewfinder;

class CameraForm : public QWidget
{
    Q_OBJECT

public:
    explicit CameraForm(QWidget *parent = 0);
    ~CameraForm();

private Q_SLOTS:
    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

private:
    Ui::CameraForm *ui;
    QCamera *camera;
    DCameraView *viewfinder;
};

#endif // FORM_H
