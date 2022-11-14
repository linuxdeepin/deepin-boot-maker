// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#ifndef QTMIPSINSTALLER_H
#define QTMIPSINSTALLER_H
#include "qtbaseinstaller.h"

class QtMipsInstaller : public QtBaseInstaller
{
public:
    QtMipsInstaller(QObject *parent = nullptr);

protected:
    virtual bool installBootload() override;
};

#endif // QTMIPSINSTALLER_H
