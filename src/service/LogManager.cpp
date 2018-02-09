/*
 * Copyright (C) 2017 ~ 2018 Wuhan Deepin Technology Co., Ltd.
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

DCORE_USE_NAMESPACE

DBMLogManager::DBMLogManager()
{
    m_format = "%{time}{yyyy-MM-dd, HH:mm:ss.zzz} [%{type:-7}] [%{file:-20} %{function:-35} %{line}] %{message}\n";
}

void DBMLogManager::initConsoleAppender()
{
    m_consoleAppender = new ConsoleAppender;
    m_consoleAppender->setFormat(m_format);
    logger->registerAppender(m_consoleAppender);
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
    logger->registerAppender(m_rollingFileAppender);
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

