/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DOBJECT_P_H
#define DOBJECT_P_H

#include "dtk_global.h"
#include "dobject.h"

DTK_BEGIN_NAMESPACE

class DObjectPrivate
{
public:
    virtual ~DObjectPrivate();

protected:
    DObjectPrivate(DObject *qq);

    DObject *q_ptr;

    Q_DECLARE_PUBLIC(DObject)
};

DTK_END_NAMESPACE

#endif // DOBJECT_P_H

