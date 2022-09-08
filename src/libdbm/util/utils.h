// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once
#include "devicemonitor.h"

#include <QObject>
#include <QString>


namespace Utils {

void initResource();
void loadTranslate();
void ClearTargetDev(const QString &targetPath);
bool CheckInstallDisk(const QString &targetDev);

QMap<QString, DeviceInfo> CommandDfParse();
QMap<QString, DeviceInfo> CommandLsblkParse();
QList<DeviceInfo>  ListUsbDrives();
QString UsbShowText(const QString &targetdev);
}
