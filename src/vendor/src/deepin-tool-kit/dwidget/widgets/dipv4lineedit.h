/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DIPV4LINEEDIT_H
#define DIPV4LINEEDIT_H

#include "dlineedit.h"

DWIDGET_BEGIN_NAMESPACE

class DIpv4LineEditPrivate;
class DIpv4LineEdit : public DLineEdit
{
    Q_OBJECT

    Q_PROPERTY(QString displayText READ displayText)
    Q_PROPERTY(int cursorPosition READ cursorPosition WRITE setCursorPosition)
    Q_PROPERTY(Qt::Alignment alignment READ alignment)
    Q_PROPERTY(QString selectedText READ selectedText)
    Q_PROPERTY(bool acceptableInput READ hasAcceptableInput)
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
    Q_PROPERTY(QString placeholderText READ placeholderText)
    Q_PROPERTY(bool clearButtonEnabled READ isClearButtonEnabled)

public:
    explicit DIpv4LineEdit(QWidget *parent = 0);

    QString displayText() const;
    int cursorPosition() const;
    Qt::Alignment alignment() const;
    bool hasAcceptableInput() const;
    bool isReadOnly() const;

public slots:
    void setCursorPosition(int cursorPosition);
    void setReadOnly(bool readOnly);
    void setSelection(int start, int length);
    void selectAll();

protected:
    bool eventFilter(QObject *obj, QEvent *e) Q_DECL_OVERRIDE;

private:
    void setPlaceholderText(QString placeholderText);
    void setClearButtonEnabled(bool clearButtonEnabled);

    D_DECLARE_PRIVATE(DIpv4LineEdit)

    Q_PRIVATE_SLOT(d_func(), void _q_updateLineEditText())
    Q_PRIVATE_SLOT(d_func(), void _q_setIpLineEditText(const QString &))
};

DWIDGET_END_NAMESPACE

#endif // DIPV4LINEEDIT_H
