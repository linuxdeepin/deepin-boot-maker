/*
 * Copyright (C) 2016 ~ 2018 Wuhan Deepin Technology Co., Ltd.
 *
 * Author:     Iceyer <me@iceyer.net>
 *
 * Maintainer: Iceyer <me@iceyer.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "LogManager.h"

#include "util/utils.h"
#include "bootmakerservice.h"

#include <QDebug>
#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusError>
#include <sanitizer/asan_interface.h>

const QString BootMakerServiceName = "com.deepin.bootmaker";
const QString BootMakerPath = "/com/deepin/bootmaker";

int main(int argc, char *argv[])
{
    QString PATH = qgetenv("PATH");

    if (PATH.isEmpty()) {
        PATH = "/usr/bin";
    }

    PATH += ":/usr/sbin";
    PATH += ":/sbin";
    qputenv("PATH", PATH.toLatin1());

    QString strDebug = qgetenv("QT_LOGGING_RULES");

    if (strDebug.isEmpty()) {
        strDebug = "*.debug=false";
    }

    qputenv("QT_LOGGING_RULES", strDebug.toLatin1());
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    Utils::initResource();

    QCoreApplication a(argc, argv);
    a.setOrganizationName("deepin");
    a.setApplicationName("deepin-boot-maker-service");
    BootMakerService service;
     __sanitizer_set_report_path("deepin-boot-maker-server.log");
    const QString m_format = "%{time}{yyyyMMdd.HH:mm:ss.zzz}[%{type:1}][%{function:-35} %{line:-4} %{threadid} ] %{message}\n";
    DBMLogManager::setSystemLog(true);
    DBMLogManager::setLogFormat(m_format);
    DBMLogManager::registerFileAppender();
    DBMLogManager::registerConsoleAppender();

    qDebug() << "write log to" << DBMLogManager::getlogFilePath();
    qDebug() << PATH;

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
