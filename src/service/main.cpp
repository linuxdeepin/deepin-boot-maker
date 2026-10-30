// SPDX-FileCopyrightText: 2016 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "util/logger.h"
#include "util/ddlog.h"
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
    // 动态日志规则（调试模式）必须在任何日志打印之前初始化；
    // MLogger 内部会读取 QT_LOGGING_RULES 环境变量作为基线并接管日志级别控制
    MLogger mlogger;

    qCInfo(serviceLog) << "Starting Boot Maker Service";

    QString PATH = qgetenv("PATH");
    qCDebug(serviceLog) << "Initial PATH:" << PATH;

    if (PATH.isEmpty()) {
        qCDebug(serviceLog) << "PATH is empty, setting default to /usr/bin";
        PATH = "/usr/bin";
    }

    PATH += ":/usr/sbin";
    PATH += ":/sbin";
    qputenv("PATH", PATH.toLatin1());
    qCDebug(serviceLog) << "Updated PATH:" << PATH;

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    // 在Qt6中，这个步骤是不必要的，因为Qt6默认使用UTF-8
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#endif
    Utils::initResource();

    QCoreApplication a(argc, argv);
    a.setOrganizationName("deepin");
    a.setApplicationName("deepin-boot-maker-service");
    qCDebug(serviceLog) << "Application initialized with name:" << a.applicationName();

    BootMakerService service;

    const QString m_format = "%{time}{yyyyMMdd.HH:mm:ss.zzz}[%{type:1}][%{function:-35} %{line:-4} %{threadid} ] %{message}\n";
    DBMLogManager::setSystemLog(true);
    DBMLogManager::setLogFormat(m_format);
    DBMLogManager::registerFileAppender();
    DBMLogManager::registerConsoleAppender();

    qCInfo(serviceLog) << "Log file path:" << DBMLogManager::getlogFilePath();
    qCDebug(serviceLog) << "Environment PATH:" << PATH;

    qCDebug(serviceLog) << "Registering D-Bus service:" << BootMakerServiceName;
    auto systemBus = QDBusConnection::systemBus();
    if (!systemBus.registerService(BootMakerServiceName)) {
        qCCritical(serviceLog) << "Failed to register D-Bus service:" << systemBus.lastError().message();
        exit(0x0001);
    }
    qCInfo(serviceLog) << "D-Bus service registered successfully";

    qCDebug(serviceLog) << "Registering D-Bus object at path:" << BootMakerPath;
    if (!systemBus.registerObject(BootMakerPath,
                                  &service,
                                  QDBusConnection::ExportAllSlots |
                                  QDBusConnection::ExportAllSignals)) {
        qCCritical(serviceLog) << "Failed to register D-Bus object:" << systemBus.lastError().message();
        exit(0x0002);
    }
    qCInfo(serviceLog) << "D-Bus object registered successfully";

    qCInfo(serviceLog) << "Boot Maker Service started successfully";
    return a.exec();
}
