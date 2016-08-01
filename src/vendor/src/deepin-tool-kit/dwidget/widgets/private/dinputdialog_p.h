/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DINPUTDIALOG_P_H
#define DINPUTDIALOG_P_H

#include "dinputdialog.h"
#include "ddialog_p.h"

DWIDGET_BEGIN_NAMESPACE

class DLabel;
class DLineEdit;
class DSpinBox;
class DDoubleSpinBox;
class DComboBox;
class DInputDialogPrivate : public DDialogPrivate
{
    DInputDialogPrivate(DInputDialog *qq);

    QAbstractButton *cancelButton = nullptr;
    QAbstractButton *okButton = nullptr;
    DLineEdit *lineEdit = nullptr;
    DSpinBox* spinBox = nullptr;
    DDoubleSpinBox *doubleSpinBox = nullptr;
    DComboBox *comboBox = nullptr;

    DInputDialog::InputMode inputMode;

    void init();

    D_DECLARE_PUBLIC(DInputDialog)
};

DWIDGET_END_NAMESPACE

#endif // DINPUTDIALOG_P_H

