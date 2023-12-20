// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#ifndef BOOTMAKERSERVICE_P_H
#define BOOTMAKERSERVICE_P_H

#include "bootmakerservice.h"

#include <backend/bootmaker.h>

class BootMakerServicePrivate
{
public:
    explicit BootMakerServicePrivate(BootMakerService *parent)
        : q_ptr(parent)
    {
    }
    ~BootMakerServicePrivate() {}
    bool checkCaller();
    qint64 dbusCallerPid();

    bool disableCheck = false;
    BootMaker *bm = nullptr;

    BootMakerService *q_ptr;
    Q_DECLARE_PUBLIC(BootMakerService)
};

#endif  // BOOTMAKERSERVICE_P_H
