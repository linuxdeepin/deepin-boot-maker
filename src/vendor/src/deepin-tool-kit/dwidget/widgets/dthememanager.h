/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DTHEMEMANAGER_H
#define DTHEMEMANAGER_H

#include <QObject>
#include <QString>
#include <QGlobalStatic>

#include "dwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class DThemeManager : public QObject
{
    Q_OBJECT

public:
    static DThemeManager * instance();

    QString theme() const;
    void setTheme(const QString theme);

    QString getQssForWidget(QString className);

public slots:
    void updateQss();

signals:
    void themeChanged(QString theme);

protected:
    DThemeManager();

    QString m_theme;
};

DWIDGET_END_NAMESPACE

#endif // DTHEMEMANAGER_H
