/*
 * Copyright (C) 2017 ~ 2018 Wuhan Deepin Technology Co., Ltd.
 *
 * Author:     Iceyer <me@iceyer.net>
 *
 * Maintainer: Iceyer <me@iceyer.net>
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

#pragma once
#include "formatdialog.h"

#include <DWidget>
#include <DFrame>
#include <DLabel>
#include <bminterface.h>

DWIDGET_USE_NAMESPACE

class DeviceInfo;
class UsbSelectView : public DWidget
{
    Q_OBJECT
public:
    explicit UsbSelectView(DWidget *parent = nullptr);

signals:
    void deviceSelected(const QString &partition, bool format);
    void finish(quint32 error, const QString &title, const QString &description);
    void backToPrevUI();
public slots:
    void getIsoFileSelectedPath(QString isoPath);

private:
    QList<DeviceInfo> m_mountDevs;
    FormatDialog formatDialogSceneA;
    FormatDialog formatDialogSceneB;
};

