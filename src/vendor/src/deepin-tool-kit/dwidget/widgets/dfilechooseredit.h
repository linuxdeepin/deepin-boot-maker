/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DFILECHOOSEREDIT_H
#define DFILECHOOSEREDIT_H

#include "dlineedit.h"

DWIDGET_BEGIN_NAMESPACE

class DFileChooserEditPrivate;
class LIBDTKWIDGETSHARED_EXPORT DFileChooserEdit : public DLineEdit
{
    Q_OBJECT

    Q_ENUMS(DialogDisplayPosition)
    Q_PROPERTY(DialogDisplayPosition dialogDisplayPosition READ dialogDisplayPosition WRITE setDialogDisplayPosition)

public:
    enum DialogDisplayPosition{
        FoloowParentWindow,
        CurrentMonitorCenter
    };


    DFileChooserEdit(QWidget *parent = nullptr);

    DialogDisplayPosition dialogDisplayPosition() const;

public Q_SLOTS:
    void setDialogDisplayPosition(DialogDisplayPosition dialogDisplayPosition);

Q_SIGNALS:
    void fileChoosed(const QString &fileName);
    void dialogOpened();
    void dialogClosed(int code);

protected:
    Q_DISABLE_COPY(DFileChooserEdit)
    D_DECLARE_PRIVATE(DFileChooserEdit)
    Q_PRIVATE_SLOT(d_func(), void _q_showFileChooserDialog())
};

DWIDGET_END_NAMESPACE

#endif // DFILECHOOSEREDIT_H
