/**
 * Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QDebug>
#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusError>

#include "LogManager.h"

#include "bootmakerservice.h"

const QString BootMakerServiceName = "com.deepin.bootmaker";
const QString BootMakerPath = "/com/deepin/bootmaker";

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    Q_INIT_RESOURCE(blob);

    QCoreApplication a(argc, argv);
    a.setOrganizationName("deepin");
    a.setApplicationName("deepin-boot-maker-service");
    BootMakerService service;

    const QString m_format = "%{time}{yyyyMMdd.HH:mm:ss.zzz}[%{type:1}][%{function:-35} %{line:-4} %{threadid} ] %{message}\n";
    DBMLogManager::setSystemLog(true);
    DBMLogManager::setLogFormat(m_format);
    DBMLogManager::registerConsoleAppender();
    DBMLogManager::registerFileAppender();

    qDebug() << "write log to" << DBMLogManager::getlogFilePath();

    auto systemBus = QDBusConnection::systemBus();
    if (!systemBus.registerService(BootMakerServiceName)) {
        qCritical() << "registerService failed:" << systemBus.lastError();
        exit(0x0001);
    }

    if (!systemBus.registerObject(BootMakerPath,
                                  &service,
                                  QDBusConnection::ExportAllSlots |
                                  QDBusConnection::ExportAllSignals)) {
        qCritical() << "registerObject failed:" << systemBus.lastError();
        exit(0x0002);
    }

    return a.exec();
}
