/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "inputtab.h"
#include "dshortcutedit.h"
#include "dfilechooseredit.h"
#include "dipv4lineedit.h"
#include "dspinbox.h"

#include <QPixmap>
#include <QDebug>

InputTab::InputTab(QWidget *parent) : QLabel(parent)
{
    setStyleSheet("InputTab{background-color: #252627;}");
    setFocusPolicy(Qt::ClickFocus);

    DTK_WIDGET_NAMESPACE::DPasswordEdit *pwdEdit = new DTK_WIDGET_NAMESPACE::DPasswordEdit(this);
    pwdEdit->setText("password");
    pwdEdit->move(20, 20);

    DTK_WIDGET_NAMESPACE::DPasswordEdit *pwdEdit2 = new DTK_WIDGET_NAMESPACE::DPasswordEdit(this);
    pwdEdit2->setText("password");
    pwdEdit2->setEchoMode(DTK_WIDGET_NAMESPACE::DPasswordEdit::Normal);
    pwdEdit2->setAlert(true);
    pwdEdit2->move(20, 50);
    connect(pwdEdit2, &DTK_WIDGET_NAMESPACE::DPasswordEdit::focusChanged, [](bool focus) {qDebug() << "focus: " << focus;});
    connect(pwdEdit2, &DTK_WIDGET_NAMESPACE::DPasswordEdit::textChanged, [](const QString &text) {qDebug() << "text: " << text;});

    DTK_WIDGET_NAMESPACE::DSearchEdit *searchEdit = new DTK_WIDGET_NAMESPACE::DSearchEdit(this);
    //searchEdit->setSearchIcon(":/images/button.png");
    //searchEdit->setFixedWidth(300);
    searchEdit->move(20, 120);
    searchEdit->setPlaceHolder("Tes");

    DTK_WIDGET_NAMESPACE::DLineEdit *lineEdit = new DTK_WIDGET_NAMESPACE::DLineEdit(this);
    lineEdit->setText("LineEdit");
    lineEdit->move(20, 180);
//    lineEdit->setAlert(true);
//    lineEdit->setFixedSize(200, 30);
    connect(lineEdit, &DTK_WIDGET_NAMESPACE::DLineEdit::focusChanged, [](bool focus) {qDebug() << "focus: " << focus;});

    DTK_WIDGET_NAMESPACE::DFileChooserEdit *fileChooser = new DTK_WIDGET_NAMESPACE::DFileChooserEdit(this);
    fileChooser->move(150, 180);
    fileChooser->setDialogDisplayPosition(DTK_WIDGET_NAMESPACE::DFileChooserEdit::CurrentMonitorCenter);

    DTK_WIDGET_NAMESPACE::DLineEdit *lineEditAlert = new DTK_WIDGET_NAMESPACE::DLineEdit(this);
    lineEditAlert->setText("AlertLineEdit");
    lineEditAlert->setAlert(true);
    lineEditAlert->setFixedSize(200, 30);
    lineEditAlert->move(20, 230);
    lineEditAlert->setValidator(new QRegExpValidator(QRegExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){0,3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)?")));

    DTK_WIDGET_NAMESPACE::DShortcutEdit *shortcutEdit = new DTK_WIDGET_NAMESPACE::DShortcutEdit(this);
    shortcutEdit->move(20, 300);

    DTK_WIDGET_NAMESPACE::DIpv4LineEdit *ipv4 = new DTK_WIDGET_NAMESPACE::DIpv4LineEdit(this);

    ipv4->move(300, 20);

    connect(ipv4, &DTK_WIDGET_NAMESPACE::DIpv4LineEdit::textChanged, this, [this, ipv4](const QString &text) {
        qDebug() << "text changed:" <<text << ipv4->text();
    });

    ipv4->setText("192.168.31.55");

    DTK_WIDGET_NAMESPACE::DSpinBox *spinbox = new DTK_WIDGET_NAMESPACE::DSpinBox(this);

    spinbox->resize(100, 22);

    spinbox->move(300, 50);

//    searchEdit->setFocus();
    lineEditAlert->setFocus();
}
