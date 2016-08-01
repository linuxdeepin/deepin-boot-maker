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

#include <QDebug>
#include <DLog>

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication a(argc, argv);
    a.setTheme("dark");
    Dtk::Util::DLogManager::registerConsoleAppender();

    if (!a.setSingleInstance("libdui-examples")) {
        qDebug() << "another instance is running!!";
    }

    // TODO: DWindow crash on windows
#ifdef Q_OS_LINUX
    MainWindow w;
#else
    DLabel w;
#endif
//    w.resize(w.size());
    w.show();

    return a.exec();
}
