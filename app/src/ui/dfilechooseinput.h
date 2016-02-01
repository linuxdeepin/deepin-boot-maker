/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DFILECHOOSEINPUT_H
#define DFILECHOOSEINPUT_H

#include <QWidget>

class QLineEdit;
class QPushButton;

class DFileChooseInput : public QWidget
{
    Q_OBJECT
public:
    explicit DFileChooseInput(QWidget *parent = 0);

    QString text() {return text_;}

    void setFixedWidth(int w);
    void setFixedHeight(int h);

signals:
    void filedSelected(const QString& filepath);

public slots:
    void selectFile();

private:
    int         btWidth_;
    QString     text_;
    QLineEdit   *edit_;
    QPushButton *bt_;
};

#endif // DFILECHOOSEINPUT_H
