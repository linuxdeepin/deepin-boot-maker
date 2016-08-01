/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DTHEMEHELPER_H
#define DTHEMEHELPER_H

#include <QObject>

class DThemeHelper : public QObject
{
    Q_OBJECT
public:
    explicit DThemeHelper(QObject *parent = 0);

    QString themeToQss(QString theme);
    QString trimThemeContents(QString qss);
};

#endif // DTHEMEHELPER_H
