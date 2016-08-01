/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DOBJECT_H
#define DOBJECT_H

#include <QScopedPointer>

#include "dtk_global.h"

DTK_BEGIN_NAMESPACE

#define D_DECLARE_PRIVATE(Class) Q_DECLARE_PRIVATE_D(qGetPtrHelper(d_d_ptr),Class)
#define D_DECLARE_PUBLIC(Class) Q_DECLARE_PUBLIC(Class)
#define D_D(Class) Q_D(Class)
#define D_Q(Class) Q_Q(Class)
#define D_DC(Class) Q_D(const Class)
#define D_QC(Class) Q_Q(const Class)
#define D_PRIVATE_SLOT(Func) Q_PRIVATE_SLOT(d_func(), Func)

class DObjectPrivate;

class LIBDTKBASESHARED_EXPORT DObject
{
protected:
    DObject(DObject * parent = nullptr);

    DObject(DObjectPrivate &dd, DObject * parent = nullptr);

    virtual ~DObject();

    QScopedPointer<DObjectPrivate> d_d_ptr;

    Q_DISABLE_COPY(DObject)
    D_DECLARE_PRIVATE(DObject)
};

DTK_END_NAMESPACE

#endif // DOBJECT_H
