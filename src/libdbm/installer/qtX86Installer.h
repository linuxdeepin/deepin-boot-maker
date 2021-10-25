/*
 * Copyright (C) 2020 ~ 2021 Uniontech Software Co., Ltd.
 *
 * Author:     shenfusheng <shenfusheng@uniontech.com>
 *
 * Maintainer: shenfusheng <shenfusheng@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
