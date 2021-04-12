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

#ifndef QTINSTALLERFACTORY_H
#define QTINSTALLERFACTORY_H

#include "qtbaseinstaller.h"

#include <QObject>

class QtInstallerFactory : public QObject
{
    Q_OBJECT
public:
    static QtInstallerFactory* getInstance();

public:
    QtBaseInstaller* createInstaller();

private:
    explicit QtInstallerFactory(QObject *parent = nullptr);

signals:

public slots:

private:
    static QtInstallerFactory* m_pSelf;
};

#endif // QTINSTALLERFACTORY_H
