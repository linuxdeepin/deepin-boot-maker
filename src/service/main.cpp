// SPDX-FileCopyrightText: 2016 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "LogManager.h"

#include "util/utils.h"
#include "bootmakerservice.h"

#include <QDebug>
#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusError>

const QString BootMakerServiceName = "com.deepin.bootmaker";
const QString BootMakerPath = "/com/deepin/bootmaker";

int main(int argc, char *argv[])
{
    qInfo() << "Starting Boot Maker Service";
    
    QString PATH = qgetenv("PATH");
    qDebug() << "Initial PATH:" << PATH;

    if (PATH.isEmpty()) {
        qDebug() << "PATH is empty, setting default to /usr/bin";
        PATH = "/usr/bin";
    }

    PATH += ":/usr/sbin";
    PATH += ":/sbin";
    qputenv("PATH", PATH.toLatin1());
    qDebug() << "Updated PATH:" << PATH;

    QString strDebug = qgetenv("QT_LOGGING_RULES");
    qDebug() << "Initial QT_LOGGING_RULES:" << strDebug;

    if (strDebug.isEmpty()) {
        qDebug() << "Setting default QT_LOGGING_RULES to *.debug=false";
        strDebug = "*.debug=false";
    }

    qputenv("QT_LOGGING_RULES", strDebug.toLatin1());
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    // 在Qt6中，这个步骤是不必要的，因为Qt6默认使用UTF-8
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#endif
    Utils::initResource();

    QCoreApplication a(argc, argv);
    a.setOrganizationName("deepin");
    a.setApplicationName("deepin-boot-maker-service");
    qDebug() << "Application initialized with name:" << a.applicationName();

    BootMakerService service;

    const QString m_format = "%{time}{yyyyMMdd.HH:mm:ss.zzz}[%{type:1}][%{function:-35} %{line:-4} %{threadid} ] %{message}\n";
    DBMLogManager::setSystemLog(true);
    DBMLogManager::setLogFormat(m_format);
    DBMLogManager::registerFileAppender();
    DBMLogManager::registerConsoleAppender();

    qInfo() << "Log file path:" << DBMLogManager::getlogFilePath();
    qDebug() << "Environment PATH:" << PATH;

    qDebug() << "Registering D-Bus service:" << BootMakerServiceName;
    auto systemBus = QDBusConnection::systemBus();
    if (!systemBus.registerService(BootMakerServiceName)) {
        qCritical() << "Failed to register D-Bus service:" << systemBus.lastError().message();
        exit(0x0001);
    }
    qInfo() << "D-Bus service registered successfully";

    qDebug() << "Registering D-Bus object at path:" << BootMakerPath;
    if (!systemBus.registerObject(BootMakerPath,
                                  &service,
                                  QDBusConnection::ExportAllSlots |
                                  QDBusConnection::ExportAllSignals)) {
        qCritical() << "Failed to register D-Bus object:" << systemBus.lastError().message();
        exit(0x0002);
    }
    qInfo() << "D-Bus object registered successfully";

    qInfo() << "Boot Maker Service started successfully";
    return a.exec();
}
