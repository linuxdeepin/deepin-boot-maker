// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "LogManager.h"

DCORE_USE_NAMESPACE

DBMLogManager::DBMLogManager()
{
    m_format = "%{time}{yyyy-MM-dd, HH:mm:ss.zzz} [%{type:-7}] [%{file:-20} %{function:-35} %{line}] %{message}\n";
}

void DBMLogManager::initConsoleAppender()
{
    m_consoleAppender = new ConsoleAppender;
    m_consoleAppender->setFormat(m_format);
#ifdef logger
    qInfo() << "defined logger!";
    logger->registerAppender(m_consoleAppender);
#else
    qInfo() << "defined dlogger!";
    dlogger->registerAppender(m_consoleAppender);
#endif
}

void DBMLogManager::initRollingFileAppender()
{
    QString cachePath = QStandardPaths::standardLocations(QStandardPaths::CacheLocation).at(0);
    if (systemLog) {
        cachePath = QString("/var/log/%1").arg(qApp->organizationName());
    }
    if (!QDir(cachePath).exists()) {
        QDir(cachePath).mkpath(cachePath);
    }
    m_logPath = joinPath(cachePath, QString("%1.log").arg(qApp->applicationName()));

    m_rollingFileAppender = new RollingFileAppender(m_logPath);
    m_rollingFileAppender->setFormat(m_format);
    m_rollingFileAppender->setLogFilesLimit(5);
    m_rollingFileAppender->setDatePattern(RollingFileAppender::DailyRollover);
#ifdef logger
    qInfo() << "defined logger!";
    logger->registerAppender(m_rollingFileAppender);
#else
    qInfo() << "defined dlogger!";
    dlogger->registerAppender(m_rollingFileAppender);
#endif
}

//! Registers the appender to write the log records to the Console
/**
 * \sa registerFileAppender
 */
void DBMLogManager::setSystemLog(bool sys)
{
    DBMLogManager::instance()->systemLog = sys;
}

void DBMLogManager::registerConsoleAppender()
{
    DBMLogManager::instance()->initConsoleAppender();
}

//! Registers the appender to write the log records to the file
/**
 * \sa getlogFilePath
 * \sa registerConsoleAppender
 */
void DBMLogManager::registerFileAppender()
{
    DBMLogManager::instance()->initRollingFileAppender();
}

//! Return the path file log storage
/**
 * \sa registerFileAppender
 */
QString DBMLogManager::getlogFilePath()
{
    return QDir::toNativeSeparators(DBMLogManager::instance()->m_logPath);
}

void DBMLogManager::setLogFormat(const QString &format)
{
    //m_format = "%{time}{yyyy-MM-dd, HH:mm:ss.zzz} [%{type:-7}] [%{file:-20} %{function:-35} %{line}] %{message}\n";
    DBMLogManager::instance()->m_format = format;
}

QString DBMLogManager::joinPath(const QString &path, const QString &fileName)
{
    QString separator(QDir::separator());
    return QString("%1%2%3").arg(path, separator, fileName);
}

DBMLogManager::~DBMLogManager()
{

}

