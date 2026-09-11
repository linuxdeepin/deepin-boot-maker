// SPDX-FileCopyrightText: 2023-2026 UnionTech Software Technology Co., Ltd.
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
    bool checkAuthorization(const QString &action);

    bool disableCheck = false;
    BootMaker *bm = nullptr;
    // Holds the ISO file descriptor forwarded by the front-end so 7z/isoinfo
    // can reach the file via /proc/self/fd/<n> under ProtectHome=true.
    QDBusUnixFileDescriptor imageFd;
    BootMakerService *q_ptr;
    Q_DECLARE_PUBLIC(BootMakerService)
};

#endif  // BOOTMAKERSERVICE_P_H
