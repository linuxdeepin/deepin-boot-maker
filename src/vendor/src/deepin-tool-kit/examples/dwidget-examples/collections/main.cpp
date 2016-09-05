/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "mainwindow.h"
#include "dlabel.h"
#include "dapplication.h"
#include "dplatformwindowhandle.h"

#include <QDebug>
#include <DLog>
#include <dutility.h>

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{

#if defined(STATIC_LIB)
    DWIDGET_INIT_RESOURCE();
#endif

    DApplication::loadDXcbPlugin();

    DApplication a(argc, argv);
    a.setTheme("light");
    Dtk::Util::DLogManager::registerConsoleAppender();

    if (!a.setSingleInstance("libdui-examples")) {
        qDebug() << "another instance is running!!";
    }

    MainWindow w;
    DUtility::moveToCenter(&w);
    w.show();

    return a.exec();
}
