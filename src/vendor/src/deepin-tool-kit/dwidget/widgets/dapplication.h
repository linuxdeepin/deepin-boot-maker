/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DAPPLICATION_H
#define DAPPLICATION_H

#include <QApplication>

#include "dwidget_global.h"
#include "dobject.h"

DWIDGET_BEGIN_NAMESPACE

class DApplicationPrivate;
class LIBDTKWIDGETSHARED_EXPORT DApplication : public QApplication, public DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DApplication)

public:
    DApplication(int &argc, char **argv);

    QString theme() const;
    void setTheme(const QString &theme);

    bool setSingleInstance(const QString &key);
    bool loadTranslator(QList<QLocale> localeFallback = QList<QLocale>() << QLocale::system());

    //! warning: Must call before QGuiApplication defined object
    static bool loadDXcbPlugin();
    static bool isDXcbPlatform();
};

DWIDGET_END_NAMESPACE

#endif // DAPPLICATION_H
