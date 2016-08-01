/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QAbstractButton>
#include <QHBoxLayout>

#include "dinputdialog.h"
#include "private/dinputdialog_p.h"
#include "dspinbox.h"
#include "dcombobox.h"
#include "dlabel.h"
#include "dpasswordedit.h"

DWIDGET_BEGIN_NAMESPACE

DInputDialogPrivate::DInputDialogPrivate(DInputDialog *qq) :
    DDialogPrivate(qq)
{

}

void DInputDialogPrivate::init()
{
    D_Q(DInputDialog);

    q->addButtons(QStringList() << QObject::tr("Cancel") << QObject::tr("Confirm"));
    q->setOnButtonClickedClose(false);
    q->setDefaultButton(1);

    cancelButton = q->getButton(0);
    okButton = q->getButton(1);

    lineEdit = new DLineEdit;
    spinBox = new DSpinBox;
    doubleSpinBox = new DDoubleSpinBox;
    comboBox = new DComboBox;

    q->addSpacing(10);
    q->addContent(lineEdit);
    q->addContent(spinBox);
    q->addContent(doubleSpinBox);
    q->addContent(comboBox);

    q->setInputMode(DInputDialog::TextInput);

    q->connect(lineEdit, &DLineEdit::textChanged, q, &DInputDialog::textValueChanged);
    q->connect(lineEdit, &DLineEdit::alertChanged, q, &DInputDialog::textAlertChanged);
    q->connect(comboBox, &DComboBox::currentTextChanged, q, &DInputDialog::textValueChanged);
    q->connect(comboBox->lineEdit(), &QLineEdit::textChanged, q, &DInputDialog::textValueChanged);
    q->connect(comboBox, SIGNAL(currentTextChanged(QString)), q, SIGNAL(textValueChanged(QString)));
    q->connect(spinBox, SIGNAL(valueChanged(int)), q, SIGNAL(intValueChanged(int)));
    q->connect(doubleSpinBox, SIGNAL(valueChanged(double)), q, SIGNAL(doubleValueChanged(double)));
    q->connect(cancelButton, &QAbstractButton::clicked, q, [q] {
        q->done(QDialog::Rejected);
        q->cancelButtonClicked();
    });
    q->connect(okButton, &QAbstractButton::clicked, q, [q, this] {
        q->done(QDialog::Accepted);
        q->okButtonClicked();

        switch (inputMode) {
        case DInputDialog::TextInput:
            q->textValueSelected(lineEdit->text());
            break;
        case DInputDialog::ComboBox:
            q->textValueSelected(comboBox->currentText());
            break;
        case DInputDialog::IntInput:
            q->intValueSelected(spinBox->value());
            break;
        case DInputDialog::DoubleInput:
            q->doubleValueSelected(doubleSpinBox->value());
            break;
        default:
            break;
        }
    });
}

DInputDialog::DInputDialog(QWidget *parent) :
    DDialog(*new DInputDialogPrivate(this), parent)
{
    d_func()->init();
}

void DInputDialog::setInputMode(DInputDialog::InputMode mode)
{
    D_D(DInputDialog);

    d->inputMode = mode;

    switch (mode) {
    case InputMode::TextInput:
        d->lineEdit->show();
        d->comboBox->hide();
        d->spinBox->hide();
        d->doubleSpinBox->hide();
        break;
    case InputMode::ComboBox:
        d->comboBox->show();
        d->lineEdit->hide();
        d->spinBox->hide();
        d->doubleSpinBox->hide();
        break;
    case InputMode::IntInput:
        d->spinBox->show();
        d->lineEdit->hide();
        d->comboBox->hide();
        d->doubleSpinBox->hide();
        break;
    case InputMode::DoubleInput:
        d->doubleSpinBox->show();
        d->lineEdit->hide();
        d->comboBox->hide();
        d->spinBox->hide();
        break;
    default:
        break;
    }
}

DInputDialog::InputMode DInputDialog::inputMode() const
{
    D_DC(DInputDialog);

    return d->inputMode;
}

void DInputDialog::setTextValue(const QString &text)
{
    D_D(DInputDialog);

    if(inputMode() == ComboBox)
        d->comboBox->setCurrentText(text);
    else
        d->lineEdit->setText(text);
}

QString DInputDialog::textValue() const
{
    D_DC(DInputDialog);

    return inputMode() == ComboBox ? d->comboBox->currentText() : d->lineEdit->text();
}

void DInputDialog::setTextEchoMode(QLineEdit::EchoMode mode)
{
    D_D(DInputDialog);

    if(mode == d->lineEdit->echoMode())
        return;

    DLineEdit *edit;

    if(mode == DLineEdit::Normal) {
        edit = new DLineEdit;
    } else {
        edit = new DPasswordEdit;

        edit->setEchoMode(mode);
    }

    disconnect(d->lineEdit, &DLineEdit::alertChanged, this, &DInputDialog::textAlertChanged);
    connect(edit, &DLineEdit::alertChanged, this, &DInputDialog::textAlertChanged);

    edit->setText(d->lineEdit->text());
    removeContent(d->lineEdit);
    addContent(edit);

    d->lineEdit = edit;
}

QLineEdit::EchoMode DInputDialog::textEchoMode() const
{
    D_DC(DInputDialog);

    return d->lineEdit->echoMode();
}

void DInputDialog::setComboBoxEditable(bool editable)
{
    D_D(DInputDialog);

    d->comboBox->setEditable(editable);
}

bool DInputDialog::isComboBoxEditable() const
{
    D_DC(DInputDialog);

    return d->comboBox->isEditable();
}

void DInputDialog::setComboBoxItems(const QStringList &items)
{
    D_D(DInputDialog);

    d->comboBox->clear();
    d->comboBox->addItems(items);
}

QStringList DInputDialog::comboBoxItems() const
{
    D_DC(DInputDialog);

    QStringList list;

    for(int i = 0; i < d->comboBox->count(); ++i) {
        list << d->comboBox->itemText(i);
    }

    return list;
}

int DInputDialog::comboBoxCurrentIndex() const
{
    D_DC(DInputDialog);

    return d->comboBox->currentIndex();
}

void DInputDialog::setComboBoxCurrentIndex(int comboBoxCurrentIndex)
{
    D_D(DInputDialog);

    d->comboBox->setCurrentIndex(comboBoxCurrentIndex);
}

void DInputDialog::setIntValue(int value)
{
    D_DC(DInputDialog);

    d->spinBox->setValue(value);
}

int DInputDialog::intValue() const
{
    D_DC(DInputDialog);

    return d->spinBox->value();
}

void DInputDialog::setIntMinimum(int min)
{
    D_D(DInputDialog);

    d->spinBox->setMinimum(min);
}

int DInputDialog::intMinimum() const
{
    D_DC(DInputDialog);

    return d->spinBox->minimum();
}

void DInputDialog::setIntMaximum(int max)
{
    D_D(DInputDialog);

    d->spinBox->setMaximum(max);
}

int DInputDialog::intMaximum() const
{
    D_DC(DInputDialog);

    return d->spinBox->maximum();
}

void DInputDialog::setIntRange(int min, int max)
{
    D_D(DInputDialog);

    d->spinBox->setRange(min, max);
}

void DInputDialog::setIntStep(int step)
{
    D_D(DInputDialog);

    d->spinBox->setSingleStep(step);
}

int DInputDialog::intStep() const
{
    D_DC(DInputDialog);

    return d->spinBox->singleStep();
}

void DInputDialog::setDoubleValue(double value)
{
    D_D(DInputDialog);

    d->doubleSpinBox->setValue(value);
}

double DInputDialog::doubleValue() const
{
    D_DC(DInputDialog);

    return d->doubleSpinBox->value();
}

void DInputDialog::setDoubleMinimum(double min)
{
    D_D(DInputDialog);

    d->doubleSpinBox->setMinimum(min);
}

double DInputDialog::doubleMinimum() const
{
    D_DC(DInputDialog);

    return d->doubleSpinBox->minimum();
}

void DInputDialog::setDoubleMaximum(double max)
{
    D_D(DInputDialog);

    d->doubleSpinBox->setMaximum(max);
}

double DInputDialog::doubleMaximum() const
{
    D_DC(DInputDialog);

    return d->doubleSpinBox->maximum();
}

void DInputDialog::setDoubleRange(double min, double max)
{
    D_D(DInputDialog);

    d->doubleSpinBox->setRange(min, max);
}

void DInputDialog::setDoubleDecimals(int decimals)
{
    D_D(DInputDialog);

    d->doubleSpinBox->setDecimals(decimals);
}

int DInputDialog::doubleDecimals() const
{
    D_DC(DInputDialog);

    return d->doubleSpinBox->decimals();
}

void DInputDialog::setOkButtonText(const QString &text)
{
    D_DC(DInputDialog);

    d->okButton->setText(text);
}

QString DInputDialog::okButtonText() const
{
    D_DC(DInputDialog);

    return d->okButton->text();
}

void DInputDialog::setCancelButtonText(const QString &text)
{
    D_DC(DInputDialog);

    d->cancelButton->setText(text);
}

QString DInputDialog::cancelButtonText() const
{
    D_DC(DInputDialog);

    return d->cancelButton->text();
}

void DInputDialog::setTextAlert(bool textAlert)
{
    D_D(DInputDialog);

    d->lineEdit->setAlert(textAlert);
}

bool DInputDialog::isTextAlert() const
{
    D_DC(DInputDialog);

    return d->lineEdit->isAlert();
}

QString DInputDialog::getText(QWidget *parent, const QString &title, const QString &message,
                              QLineEdit::EchoMode echo, const QString &text, bool *ok,
                              Qt::WindowFlags flags, Qt::InputMethodHints inputMethodHints)
{
    DInputDialog dialog(parent);

    dialog.setTitle(title);
    dialog.setMessage(message);
    dialog.setTextEchoMode(echo);
    dialog.setTextValue(text);
    dialog.setWindowFlags(flags);
    dialog.setInputMethodHints(inputMethodHints);

    *ok = dialog.exec() == QDialog::Accepted;

    return dialog.textValue();
}

QString DInputDialog::getItem(QWidget *parent, const QString &title, const QString &message,
                              const QStringList &items, int current, bool editable, bool *ok,
                              Qt::WindowFlags flags, Qt::InputMethodHints inputMethodHints)
{
    DInputDialog dialog(parent);

    dialog.setComboBoxItems(items);
    dialog.setComboBoxEditable(editable);
    dialog.setComboBoxCurrentIndex(current);
    dialog.setTitle(title);
    dialog.setMessage(message);
    dialog.setWindowFlags(flags);
    dialog.setInputMethodHints(inputMethodHints);

    *ok = dialog.exec() == QDialog::Accepted;

    return dialog.textValue();
}

int DInputDialog::getInt(QWidget *parent, const QString &title, const QString &message,
                         int value, int minValue, int maxValue, int step, bool *ok,
                         Qt::WindowFlags flags)
{
    DInputDialog dialog(parent);

    dialog.setIntValue(value);
    dialog.setIntRange(minValue, maxValue);
    dialog.setIntStep(step);
    dialog.setTitle(title);
    dialog.setMessage(message);
    dialog.setWindowFlags(flags);

    *ok = dialog.exec() == QDialog::Accepted;

    return dialog.intValue();
}

double DInputDialog::getDouble(QWidget *parent, const QString &title, const QString &message,
                               double value, double minValue, double maxValue, int decimals,
                               bool *ok, Qt::WindowFlags flags)
{
    DInputDialog dialog(parent);

    dialog.setDoubleValue(value);
    dialog.setDoubleRange(minValue, maxValue);
    dialog.setDoubleDecimals(decimals);
    dialog.setTitle(title);
    dialog.setMessage(message);
    dialog.setWindowFlags(flags);

    *ok = dialog.exec() == QDialog::Accepted;

    return dialog.doubleValue();
}

DWIDGET_END_NAMESPACE
