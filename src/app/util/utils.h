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

#include "usbdevicemonitor.h"

namespace Utils
{

QMap<QString, DeviceInfo> CommandDfParse();
QMap<QString, DeviceInfo> CommandLsblkParse();

QString UsbShowText(const QString &targetdev);
void ClearTargetDev(const QString &targetPath);
QList<DeviceInfo>  ListUsbDrives();
bool CheckInstallDisk(const QString &targetDev);
}
