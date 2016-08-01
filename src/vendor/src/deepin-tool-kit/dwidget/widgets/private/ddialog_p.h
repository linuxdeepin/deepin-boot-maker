/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DDIALOG_P_H
#define DDIALOG_P_H

#include <QPointer>
#include "ddialog.h"
#include "dabstractdialogprivate_p.h"

class QHBoxLayout;

DWIDGET_BEGIN_NAMESPACE

class DVBoxWidget;
class DHBoxWidget;
class DDialogPrivate : public DAbstractDialogPrivate
{
protected:
    DDialogPrivate(DDialog *qq);

private:
    bool onButtonClickedClose = true;
    QIcon icon;
    QString title;
    QString message;
    Qt::TextFormat textFormat;

    QLabel* iconLabel;
    QLabel* messageLabel;
    QLabel* titleLabel;

    QPushButton* closeButton;
    QHBoxLayout *iconLayout;
    QVBoxLayout *contentLayout;
    QHBoxLayout *buttonLayout;
    const QScreen *targetScreen = nullptr;

    QList<QAbstractButton*> buttonList;
    QList<QWidget*> contentList;

    QPointer<QAbstractButton> defaultButton;

    int clickedButtonIndex;

    void init();
    const QScreen *getScreen() const;

    void _q_onButtonClicked();
    void _q_updateLabelMaxWidth();
    void _q_defaultButtonTriggered();

    Q_DECLARE_PUBLIC(DDialog)
};

DWIDGET_END_NAMESPACE

#endif // DDIALOG_P_H

