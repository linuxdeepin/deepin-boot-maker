/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#pragma once
#include <QObject>
#include <QString>

#include "devicemonitor.h"

namespace Utils
{

void loadTranslate();
void ClearTargetDev(const QString &targetPath);
bool CheckInstallDisk(const QString &targetDev);

QMap<QString, DeviceInfo> CommandDfParse();
QMap<QString, DeviceInfo> CommandLsblkParse();
QList<DeviceInfo>  ListUsbDrives();

QString UsbShowText(const QString &targetdev);
}
