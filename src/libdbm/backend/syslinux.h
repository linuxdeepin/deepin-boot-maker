// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#ifndef SYSLINUX_H
#define SYSLINUX_H

#include <QObject>

class Syslinux : public QObject
{
    Q_OBJECT
public:
    explicit Syslinux(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SYSLINUX_H
