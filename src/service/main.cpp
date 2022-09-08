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
