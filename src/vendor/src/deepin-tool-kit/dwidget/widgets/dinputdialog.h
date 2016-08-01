/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DINPUTDIALOG_H
#define DINPUTDIALOG_H

#include <QInputDialog>

#include "ddialog.h"
#include "dlineedit.h"
#include "dobject.h"

DWIDGET_BEGIN_NAMESPACE

class DInputDialogPrivate;
class DInputDialog : public DDialog
{
    Q_OBJECT

    Q_PROPERTY(InputMode inputMode READ inputMode WRITE setInputMode)
    Q_PROPERTY(QString textValue READ textValue WRITE setTextValue NOTIFY textValueChanged)
    Q_PROPERTY(int intValue READ intValue WRITE setIntValue NOTIFY intValueChanged)
    Q_PROPERTY(int doubleValue READ doubleValue WRITE setDoubleValue NOTIFY doubleValueChanged)
    Q_PROPERTY(DLineEdit::EchoMode textEchoMode READ textEchoMode WRITE setTextEchoMode)
    Q_PROPERTY(bool comboBoxEditable READ isComboBoxEditable WRITE setComboBoxEditable)
    Q_PROPERTY(QStringList comboBoxItems READ comboBoxItems WRITE setComboBoxItems)
    Q_PROPERTY(int comboBoxCurrentIndex READ comboBoxCurrentIndex WRITE setComboBoxCurrentIndex NOTIFY comboBoxCurrentIndexChanged)
    Q_PROPERTY(int intMinimum READ intMinimum WRITE setIntMinimum)
    Q_PROPERTY(int intMaximum READ intMaximum WRITE setIntMaximum)
    Q_PROPERTY(int intStep READ intStep WRITE setIntStep)
    Q_PROPERTY(double doubleMinimum READ doubleMinimum WRITE setDoubleMinimum)
    Q_PROPERTY(double doubleMaximum READ doubleMaximum WRITE setDoubleMaximum)
    Q_PROPERTY(int doubleDecimals READ doubleDecimals WRITE setDoubleDecimals)
    Q_PROPERTY(QString okButtonText READ okButtonText WRITE setOkButtonText)
    Q_PROPERTY(QString cancelButtonText READ cancelButtonText WRITE setCancelButtonText)
    Q_PROPERTY(bool textAlert READ isTextAlert WRITE setTextAlert NOTIFY textAlertChanged)

public:
    enum InputMode {
        TextInput,
        ComboBox,
        IntInput,
        DoubleInput
    };

    explicit DInputDialog(QWidget *parent = 0);

    Q_SLOT void setInputMode(InputMode mode);
    InputMode inputMode() const;

    Q_SLOT void setTextValue(const QString &text);
    QString textValue() const;

    Q_SLOT void setTextEchoMode(QLineEdit::EchoMode mode);
    QLineEdit::EchoMode textEchoMode() const;

    Q_SLOT void setComboBoxEditable(bool editable);
    bool isComboBoxEditable() const;

    Q_SLOT void setComboBoxItems(const QStringList &items);
    QStringList comboBoxItems() const;

    Q_SLOT void setComboBoxCurrentIndex(int comboBoxCurrentIndex);
    int comboBoxCurrentIndex() const;

    Q_SLOT void setIntValue(int value);
    int intValue() const;

    Q_SLOT void setIntMinimum(int min);
    int intMinimum() const;

    Q_SLOT void setIntMaximum(int max);
    int intMaximum() const;

    Q_SLOT void setIntRange(int min, int max);

    Q_SLOT void setIntStep(int step);
    int intStep() const;

    Q_SLOT void setDoubleValue(double value);
    double doubleValue() const;

    Q_SLOT void setDoubleMinimum(double min);
    double doubleMinimum() const;

    Q_SLOT void setDoubleMaximum(double max);
    double doubleMaximum() const;

    Q_SLOT void setDoubleRange(double min, double max);

    Q_SLOT void setDoubleDecimals(int decimals);
    int doubleDecimals() const;

    Q_SLOT void setOkButtonText(const QString &text);
    QString okButtonText() const;

    Q_SLOT void setCancelButtonText(const QString &text);
    QString cancelButtonText() const;

    Q_SLOT void setTextAlert(bool textAlert);
    bool isTextAlert() const;

    static QString getText(QWidget *parent, const QString &title, const QString &message,
                           QLineEdit::EchoMode echo = QLineEdit::Normal,
                           const QString &text = QString(), bool *ok = 0, Qt::WindowFlags flags = 0,
                           Qt::InputMethodHints inputMethodHints = Qt::ImhNone);

    static QString getItem(QWidget *parent, const QString &title, const QString &message,
                           const QStringList &items, int current = 0, bool editable = true,
                           bool *ok = 0, Qt::WindowFlags flags = 0,
                           Qt::InputMethodHints inputMethodHints = Qt::ImhNone);

    static int getInt(QWidget *parent, const QString &title, const QString &message, int value = 0,
                      int minValue = -2147483647, int maxValue = 2147483647,
                      int step = 1, bool *ok = 0, Qt::WindowFlags flags = 0);
    static double getDouble(QWidget *parent, const QString &title, const QString &message, double value = 0,
                            double minValue = -2147483647, double maxValue = 2147483647,
                            int decimals = 1, bool *ok = 0, Qt::WindowFlags flags = 0);

#if QT_DEPRECATED_SINCE(5, 0)
    QT_DEPRECATED static inline int getInteger(QWidget *parent, const QString &title, const QString &label, int value = 0,
                          int minValue = -2147483647, int maxValue = 2147483647,
                          int step = 1, bool *ok = 0, Qt::WindowFlags flags = 0)
    {
        return getInt(parent, title, label, value, minValue, maxValue, step, ok, flags);
    }
#endif

Q_SIGNALS:
    // ### emit signals!
    void textValueChanged(const QString &text);
    void textValueSelected(const QString &text);
    void intValueChanged(int value);
    void intValueSelected(int value);
    void doubleValueChanged(double value);
    void doubleValueSelected(double value);
    void cancelButtonClicked();
    void okButtonClicked();
    void comboBoxCurrentIndexChanged(int comboBoxCurrentIndex);
    void textAlertChanged(bool textAlert);

private:
    D_DECLARE_PRIVATE(DInputDialog)
};

DWIDGET_END_NAMESPACE

#endif // DINPUTDIALOG_H
