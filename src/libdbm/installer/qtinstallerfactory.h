// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

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
