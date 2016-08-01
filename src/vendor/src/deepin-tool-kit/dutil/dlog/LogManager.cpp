#include "LogManager.h"
#include <Logger.h>
#include <ConsoleAppender.h>
#include <RollingFileAppender.h>

DUTIL_BEGIN_NAMESPACE

/**
 * \class DLogManager
 *
 * \brief DLogManager is the deepin user application log manager
 */

DLogManager::DLogManager()
{
    QString cachePath = QStandardPaths::standardLocations(QStandardPaths::CacheLocation).at(0);
    if (!QDir(cachePath).exists()){
        QDir(cachePath).mkpath(cachePath);
    }
    m_logPath = joinPath(cachePath, QString("%1.log").arg(qApp->applicationName()));
    m_format = "%{time}{yyyy-MM-dd, HH:mm:ss.zzz} [%{type:-7}] [%{file:-20} %{function:-35} %{line}] %{message}\n";
}

void DLogManager::initConsoleAppender(){
    m_consoleAppender = new ConsoleAppender;
    m_consoleAppender->setFormat(m_format);
    logger->registerAppender(m_consoleAppender);
}

void DLogManager::initRollingFileAppender(){
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
void DLogManager::registerConsoleAppender(){
    DLogManager::instance()->initConsoleAppender();
}

//! Registers the appender to write the log records to the file
/**
 * \sa getlogFilePath
 * \sa registerConsoleAppender
 */
void DLogManager::registerFileAppender() {
    DLogManager::instance()->initRollingFileAppender();
}

//! Return the path file log storage
/**
 * \sa registerFileAppender
 */
QString DLogManager::getlogFilePath(){
    return DLogManager::instance()->m_logPath;
}

void DLogManager::setLogFormat(const QString &format)
{
    //m_format = "%{time}{yyyy-MM-dd, HH:mm:ss.zzz} [%{type:-7}] [%{file:-20} %{function:-35} %{line}] %{message}\n";
    DLogManager::instance()->m_format = format;
}

QString DLogManager::joinPath(const QString &path, const QString &fileName){
    QString separator(QDir::separator());
    return QString("%1%2%3").arg(path, separator, fileName);
}

DLogManager::~DLogManager()
{

}

DUTIL_END_NAMESPACE
