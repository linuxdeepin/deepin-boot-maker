/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dthemehelper.h"

DThemeHelper::DThemeHelper(QObject *parent) :
    QObject(parent)
{

}

QString DThemeHelper::themeToQss(QString theme)
{
    return theme;
}

QString DThemeHelper::trimThemeContents(QString qss)
{
    return qss;
}
