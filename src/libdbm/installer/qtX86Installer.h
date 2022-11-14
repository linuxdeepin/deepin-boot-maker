// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#ifndef QTLINUXINSTALLER_H
#define QTLINUXINSTALLER_H

#include "qtbaseinstaller.h"

#include <QObject>

class QtX86Installer : public QtBaseInstaller
{
    Q_OBJECT
public:
    explicit QtX86Installer(QObject *parent = nullptr);
    virtual ~QtX86Installer() {

    }

protected:
    virtual bool installBootload() override;

signals:

public slots:
};

#endif // QTLINUXINSTALLER_H
