/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dfilechooseredit.h"
#include "dthememanager.h"
#include "private/dfilechooseredit_p.h"

#include <QDebug>
#include <QFileDialog>
#include <QScreen>
#include <QGuiApplication>

DWIDGET_BEGIN_NAMESPACE

DFileChooserEdit::DFileChooserEdit(QWidget *parent)
    : DLineEdit(*new DFileChooserEditPrivate(this), parent)
{
    D_THEME_INIT_WIDGET(DFileChooserEdit);
    D_D(DFileChooserEdit);

    d->init();
}

DFileChooserEdit::DialogDisplayPosition DFileChooserEdit::dialogDisplayPosition() const
{
    D_DC(DFileChooserEdit);

    return d->dialogDisplayPosition;
}

void DFileChooserEdit::setDialogDisplayPosition(DFileChooserEdit::DialogDisplayPosition dialogDisplayPosition)
{
    D_D(DFileChooserEdit);

    d->dialogDisplayPosition = dialogDisplayPosition;
}

DFileChooserEditPrivate::DFileChooserEditPrivate(DFileChooserEdit *q)
    : DLineEditPrivate(q)
{
}

void DFileChooserEditPrivate::init()
{
    D_Q(DFileChooserEdit);

    q->setTextMargins(0, 0, 16, 0);
    q->setReadOnly(true);
    q->setIconVisible(true);
    q->connect(q, SIGNAL(iconClicked()), q, SLOT(_q_showFileChooserDialog()));
}

void DFileChooserEditPrivate::_q_showFileChooserDialog()
{
    D_Q(DFileChooserEdit);

    QFileDialog dialog(q);

    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);

    if(dialogDisplayPosition == DFileChooserEdit::CurrentMonitorCenter) {
        QPoint pos = QCursor::pos();

        for(QScreen *screen : qApp->screens()) {
            if(screen->geometry().contains(pos)) {
                QRect rect = dialog.geometry();
                rect.moveCenter(screen->geometry().center());
                dialog.move(rect.topLeft());
            }
        }
    }

    q->dialogOpened();

    int code = dialog.exec();


    if(code == QDialog::Accepted && !dialog.selectedFiles().isEmpty()) {
        const QString fileName = dialog.selectedFiles().first();

        q->setText(fileName);
        emit q->fileChoosed(fileName);
    }

    q->dialogClosed(code);
}

DWIDGET_END_NAMESPACE

#include "moc_dfilechooseredit.cpp"
