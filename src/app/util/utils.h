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

namespace Utils
{
QString UsbShowText(const QString &targetdev);
void ClearTargetDev(const QString &targetPath);
QStringList ListUsbDrives();
bool CheckInstallDisk(const QString &targetDev);
}
