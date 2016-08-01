/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QFile>
#include <QWidget>

#include "dthememanager.h"
#include "dapplication.h"
#include "private/dthemehelper.h"

DWIDGET_BEGIN_NAMESPACE

class DThemeManagerPrivate : public DThemeManager
{

};

Q_GLOBAL_STATIC(DThemeManagerPrivate, DThemeManagerStatic)

DThemeManager * DThemeManager::instance()
{
    return DThemeManagerStatic;
}

QString DThemeManager::theme() const
{
    return m_theme;
}

void DThemeManager::setTheme(const QString theme)
{
    if (m_theme != theme) {
        m_theme = theme;

        emit themeChanged(theme);
    }
}

QString DThemeManager::getQssForWidget(QString className)
{
    QString qss;

    QFile themeFile(QString(":/%1/%2.theme").arg(m_theme).arg(className));

    if (themeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qss = themeFile.readAll();

        themeFile.close();
    }

    return qss;
}

void DThemeManager::updateQss()
{
    QWidget *w = qobject_cast<QWidget*>(sender());
    if(w){
        w->setStyleSheet(w->styleSheet());
    }
}

DThemeManager::DThemeManager() :
    QObject()
{
    this->setTheme("dark");
}


DWIDGET_END_NAMESPACE
