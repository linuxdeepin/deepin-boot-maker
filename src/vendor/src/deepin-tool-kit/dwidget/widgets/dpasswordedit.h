/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DPASSWORDEDIT_H
#define DPASSWORDEDIT_H

#include "dlineedit.h"

DWIDGET_BEGIN_NAMESPACE

class DPasswordEditPrivate;
class LIBDTKWIDGETSHARED_EXPORT DPasswordEdit : public DLineEdit
{
    Q_OBJECT
    Q_PROPERTY(bool isEchoMode READ isEchoMode)

public:
    DPasswordEdit(QWidget *parent = nullptr);

    bool isEchoMode() const;
    void setEchoMode(EchoMode mode);

protected:
    Q_DISABLE_COPY(DPasswordEdit)
    D_DECLARE_PRIVATE(DPasswordEdit)
    Q_PRIVATE_SLOT(d_func(), void _q_toggleEchoMode())
};

DWIDGET_END_NAMESPACE

#endif // DPASSWORDEDIT_H
