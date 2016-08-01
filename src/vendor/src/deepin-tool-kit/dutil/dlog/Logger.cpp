/*
  Copyright (c) 2012 Boris Moiseev (cyberbobs at gmail dot com)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License version 2.1
  as published by the Free Software Foundation and appearing in the file
  LICENSE.LGPL included in the packaging of this file.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.
*/
// Local
#include "Logger.h"
#include "AbstractAppender.h"
#include "AbstractStringAppender.h"

// Qt
#include <QCoreApplication>
#include <QReadWriteLock>
#include <QSemaphore>
#include <QDateTime>
#include <QIODevice>
#include <QTextCodec>

#if defined(Q_OS_ANDROID)
#  include <android/log.h>
#  include <AndroidAppender.h>
#endif

// STL
#include <iostream>

DUTIL_BEGIN_NAMESPACE

/**
 * \file Logger.h
 * \brief A file containing the description of Logger class and and additional useful macros for logging
 */

/**
 * \def logger
 *
 * \brief Macro returning the current instance of Logger object
 *
 * If you haven't created a local Logger object it returns the same value as the Logger::globalInstance() functions.
 * This macro is a recommended way to get an access to the Logger instance used in current class.
 *
 * Example:
 * \code
 * ConsoleAppender* consoleAppender = new ConsoleAppender;
 * logger->registerAppender(consoleAppender);
 * \endcode
 *
 * \sa Dtk::Logger::globalInstance()
 */


/**
 * \def dTrace
 *
 * \brief Writes the trace log record
 *
 * This macro is the convinient way to call Logger::write(). It uses the common preprocessor macros \c __FILE__,
 * \c __LINE__ and the standart Qt \c Q_FUNC_INFO macros to automatically determine the needed parameters to call
 * Logger::write().
 *
 * \note This and other (dInfo() etc...) macros uses the variadic macro arguments to give convinient usage form for
 * the different versions of Logger::write() (using the QString or const char* argument or returning the QDebug class
 * instance). Not all compilers will support this. Please, consider reviewing your compiler documentation to ensure
 * it support __VA_ARGS__ macro.
 *
 * \sa Dtk::Logger::LogLevel
 * \sa Dtk::Logger::write()
 */


/**
 * \def dDebug
 *
 * \brief Writes the debug log record
 *
 * This macro records the debug log record using the Logger::write() function. It works similar to the dTrace()
 * macro.
 *
 * \sa dTrace()
 * \sa Dtk::Logger::LogLevel
 * \sa Dtk::Logger::write()
 */


/**
 * \def dInfo
 *
 * \brief Writes the info log record
 *
 * This macro records the info log record using the Logger::write() function. It works similar to the dTrace()
 * macro.
 *
 * \sa dTrace()
 * \sa Dtk::Logger::LogLevel
 * \sa Dtk::Logger::write()
 */


/**
 * \def dWarning
 *
 * \brief Write the warning log record
 *
 * This macro records the warning log record using the Logger::write() function. It works similar to the dTrace()
 * macro.
 *
 * \sa dTrace()
 * \sa Dtk::Logger::LogLevel
 * \sa Dtk::Logger::write()
 */


/**
 * \def dError
 *
 * \brief Write the error log record
 * This macro records the error log record using the Logger::write() function. It works similar to the dTrace()
 * macro.
 *
 * \sa dTrace()
 * \sa Dtk::Logger::LogLevel
 * \sa Dtk::Logger::write()
 */


/**
 * \def dFatal
 *
 * \brief Write the fatal log record
 *
 * This macro records the fatal log record using the Logger::write() function. It works similar to the dTrace()
 * macro.
 *
 * \note Recording of the log record using the Logger::Fatal log level will lead to calling the STL abort()
 *       function, which will interrupt the running of your software and begin the writing of the core dump.
 *
 * \sa dTrace()
 * \sa Dtk::Logger::LogLevel
 * \sa Dtk::Logger::write()
 */


/**
 * \def dCTrace(category)
 *
 * \brief Writes the trace log record to the specific category
 *
 * This macro is the similar to the dTrace() macro, but has a category parameter
 * to write only to the category appenders (registered using Logger::registerCategoryAppender() method).
 *
 * \param category category name string
 *
 * \sa dTrace()
 * \sa Dtk::Logger::LogLevel
 * \sa Dtk::Logger::registerCategoryAppender()
 * \sa Dtk::Logger::write()
 * \sa dCategory(), dGlobalCategory()
 */


/**
 * \def dCDebug
 *
 * \brief Writes the debug log record to the specific category
 *
 * This macro records the debug log record using the Logger::write() function. It works similar to the dCTrace()
 * macro.
 *
 * \sa dCTrace()
 */


/**
 * \def dCInfo
 *
 * \brief Writes the info log record to the specific category
 *
 * This macro records the info log record using the Logger::write() function. It works similar to the dCTrace()
 * macro.
 *
 * \sa dCTrace()
 */


/**
 * \def dCWarning
 *
 * \brief Writes the warning log record to the specific category
 *
 * This macro records the warning log record using the Logger::write() function. It works similar to the dCTrace()
 * macro.
 *
 * \sa dCTrace()
 */


/**
 * \def dCError
 *
 * \brief Writes the error log record to the specific category
 *
 * This macro records the error log record using the Logger::write() function. It works similar to the dCTrace()
 * macro.
 *
 * \sa dCTrace()
 */


/**
 * \def dCFatal
 *
 * \brief Write the fatal log record to the specific category
 *
 * This macro records the fatal log record using the Logger::write() function. It works similar to the dCTrace()
 * macro.
 *
 * \note Recording of the log record using the Logger::Fatal log level will lead to calling the STL abort()
 *       function, which will interrupt the running of your software and begin the writing of the core dump.
 *
 * \sa dCTrace()
 */


/**
 * \def dCategory(category)
 *
 * \brief Create logger instance inside your custom class to log all messages to the specified category
 *
 * This macro is used to pass all log messages inside your custom class to the specific category.
 * You must include this macro inside your class declaration (similarly to the Q_OBJECT macro).
 * Internally, this macro redefines loggerInstance() function, creates the local Logger object inside your class and
 * sets the default category to the specified parameter.
 *
 * Thus, any call to loggerInstance() (for example, inside dTrace() macro) will return the local Logger object,
 * so any logging message will be directed to the default category.
 *
 * \note This macro does not register any appender to the newly created logger instance. You should register
 * logger appenders manually, inside your class.
 *
 * Usage example:
 * \code
 * class CustomClass : public QObject
 * {
 *   Q_OBJECT
 *   dCategory("custom_category")
 *   ...
 * };
 *
 * CustomClass::CustomClass(QObject* parent) : QObject(parent)
 * {
 *   logger->registerAppender(new FileAppender("custom_category_log"));
 *   dTrace() << "Trace to the custom category log";
 * }
 * \endcode
 *
 * \sa Dtk::Logger::write()
 * \sa dTrace
 * \sa Dtk::Logger::registerCategoryAppender()
 * \sa Dtk::Logger::setDefaultCategory()
 * \sa dGlobalCategory
 */


/**
 * \def dGlobalCategory(category)
 *
 * \brief Create logger instance inside your custom class to log all messages both to the specified category and to
 * the global logger instance.
 *
 * This macro is similar to dCategory(), but also passes all log messages to the global logger instance appenders.
 * It is equal to defining the local category logger using dCategory macro and calling:
 * \code logger->logToGlobalInstance(logger->defaultCategory(), true); \endcode
 *
 * \sa dCategory
 * \sa Dtk::Logger::logToGlobalInstance()
 * \sa Dtk::Logger::defaultCategory()
 * \sa Dtk::Logger::registerCategoryAppender()
 * \sa Dtk::Logger::write()
 */



/**
 * \def dAssert
 *
 * \brief Check the assertion
 *
 * This macro is a convinient and recommended to use way to call Logger::writeAssert() function. It uses the
 * preprocessor macros (as the dDebug() does) to fill the necessary arguments of the Logger::writeAssert() call. It
 * also uses undocumented but rather mature and stable \c qt_noop() function (which does nothing) when the assertion
 * is true.
 *
 * Example:
 * \code
 * bool b = checkSomething();
 * ...
 * dAssert(b == true);
 * \endcode
 *
 * \sa Dtk::Logger::writeAssert()
 */


/**
 * \def dTraceTime
 *
 * \brief Logs the processing time of current function / code block
 *
 * This macro automagically measures the function or code of block execution time and outputs it as a Logger::Trace
 * level log record.
 *
 * Example:
 * \code
 * int foo()
 * {
 *   dTraceTime();
 *   ... // Do some long operations
 *   return 0;
 * } // Outputs: Function foo finished in <time> ms.
 * \endcode
 *
 * If you are measuring a code of block execution time you may also add a name of block to the macro:
 * \code
 * int bar(bool doFoo)
 * {
 *   dTraceTime();
 *
 *   if (doFoo)
 *   {
 *     dTraceTime("Foo");
 *     ...
 *   }
 *
 *   ...
 * }
 * // Outputs:
 * // "Foo" finished in <time1> ms.
 * // Function bar finished in <time2> ms.
 * \endcode
 *
 * \note Macro switches to logging the seconds instead of milliseconds when the execution time reaches 10000 ms.
 * \sa dDebugTime, dInfoTime
 */


/**
 * \def dDebugTime
 *
 * \brief Logs the processing time of current function / code block
 *
 * This macro automagically measures the function or code of block execution time and outputs it as a Logger::Debug
 * level log record. It works similar to dTraceTime() macro.
 *
 * \sa dTraceTime
 */


/**
 * \def dInfoTime
 *
 * \brief Logs the processing time of current function / code block
 *
 * This macro automagically measures the function or code of block execution time and outputs it as a Logger::Info
 * level log record. It works similar to dTraceTime() macro.
 *
 * \sa dTraceTime
 */


/**
 * \class Logger
 *
 * \brief Very simple but rather powerful component which may be used for logging your application activities.
 *
 * Global logger instance created on a first access to it (e.g. registering appenders, calling a dDebug() macro
 * etc.) registers itself as a Qt default message handler and captures all the qDebug/dWarning/qCritical output.
 *
 * \note Qt 4 qDebug set of macro doesn't support capturing source function name, file name or line number so we
 *       recommend to use dDebug() and other Logger macros instead.
 *
 * \sa logger
 * \sa [DLogger Documentation](index.html)
 */

class LogDevice : public QIODevice
{
  public:
    LogDevice(Logger* l)
      : m_logger(l),
        m_semaphore(1)
    {}

    void lock(Logger::LogLevel logLevel, const char* file, int line, const char* function, const char* category)
    {
      m_semaphore.acquire();

      if (!isOpen())
        open(QIODevice::WriteOnly);

      m_logLevel = logLevel;
      m_file = file;
      m_line = line;
      m_function = function;
      m_category = category;
    }

  protected:
    qint64 readData(char*, qint64)
    {
      return 0;
    }

    qint64 writeData(const char* data, qint64 maxSize)
    {
      if (maxSize > 0)
        m_logger->write(m_logLevel, m_file, m_line, m_function, m_category, QString::fromLocal8Bit(QByteArray(data, maxSize)));

      m_semaphore.release();
      return maxSize;
    }

  private:
    Logger* m_logger;
    QSemaphore m_semaphore;
    Logger::LogLevel m_logLevel;
    const char* m_file;
    int m_line;
    const char* m_function;
    const char* m_category;
};


// Forward declarations
static void cleanupLoggerGlobalInstance();

#if QT_VERSION >= 0x050000
static void qtLoggerMessageHandler(QtMsgType, const QMessageLogContext& context, const QString& msg);
#else
static void qtLoggerMessageHandler(QtMsgType type, const char* msg);
#endif

/**
 * \internal
 *
 * LoggerPrivate class implements the Singleton pattern in a thread-safe way. It contains a static pointer to the
 * global logger instance protected by QReadWriteLock
 */
class LoggerPrivate
{
  public:
    static Logger* globalInstance;
    static QReadWriteLock globalInstanceLock;

    QList<AbstractAppender*> appenders;
    QMutex loggerMutex;

    QMap<QString, bool> categories;
    QMultiMap<QString, AbstractAppender*> categoryAppenders;
    QString defaultCategory;

    LogDevice* logDevice;
};


// Static fields initialization
Logger* LoggerPrivate::globalInstance = 0;
QReadWriteLock LoggerPrivate::globalInstanceLock;


static void cleanupLoggerGlobalInstance()
{
  QWriteLocker locker(&LoggerPrivate::globalInstanceLock);

  delete LoggerPrivate::globalInstance;
  LoggerPrivate::globalInstance = 0;
}


#if QT_VERSION >= 0x050000
static void qtLoggerMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
  Logger::LogLevel level;
  switch (type)
  {
    case QtDebugMsg:
      level = Logger::Debug;
      break;
#if QT_VERSION >= 0x050500
    case QtInfoMsg:
      level = Logger::Info;
      break;
#endif
    case QtWarningMsg:
      level = Logger::Warning;
      break;
    case QtCriticalMsg:
      level = Logger::Error;
      break;
    case QtFatalMsg:
      level = Logger::Fatal;
      break;
  default:
      level = Logger::Warning;
      break;
  }

  bool isDefaultCategory = QString::fromLatin1(context.category) == "default";
  Logger::globalInstance()->write(level, context.file, context.line, context.function, isDefaultCategory ? 0 : context.category, msg);
}

#else

static void qtLoggerMessageHandler(QtMsgType type, const char* msg)
{
  switch (type)
  {
    case QtDebugMsg:
      loggerInstance()->write(Logger::Debug, "", 0, "qDebug", 0, msg);
      break;
    case QtWarningMsg:
      loggerInstance()->write(Logger::Warning, "", 0, "qDebug", 0, msg);
      break;
    case QtCriticalMsg:
      loggerInstance()->write(Logger::Error, "", 0, "qDebug", 0, msg);
      break;
    case QtFatalMsg:
      loggerInstance()->write(Logger::Fatal, "", 0, "qDebug", 0, msg);
      break;
  }
}
#endif


//! Construct the instance of Logger
/**
 * If you're only using one global instance of logger you wouldn't probably need to use this constructor manually.
 * Consider using [logger](@ref logger) macro instead to access the logger instance
 */
Logger::Logger()
  : d_ptr(new LoggerPrivate)
{
  Q_D(Logger);

  d->logDevice = new LogDevice(this);
}


//! Construct the instance of Logger and set logger default category
/**
 * If you're only using one global instance of logger you wouldn't probably need to use this constructor manually.
 * Consider using [logger](@ref logger) macro instead to access the logger instance and call
 * [setDefaultCategory](@ref setDefaultCategory) method.
 *
 * \sa Logger()
 * \sa setDefaultCategory()
 */
Logger::Logger(const QString& defaultCategory)
  : d_ptr(new LoggerPrivate)
{
  Q_D(Logger);
  d->logDevice = new LogDevice(this);

  setDefaultCategory(defaultCategory);
}


//! Destroy the instance of Logger
/**
 * You probably wouldn't need to use this function directly. Global instance of logger will be destroyed automatically
 * at the end of your QCoreApplication execution
 */
Logger::~Logger()
{
  Q_D(Logger);

  // Cleanup appenders
  QMutexLocker appendersLocker(&d->loggerMutex);
  qDeleteAll(d->appenders);
  qDeleteAll(d->categoryAppenders);

  // Cleanup device
  delete d->logDevice;
  appendersLocker.unlock();

  delete d_ptr;
}


//! Converts the LogLevel enum value to its string representation
/**
 * \param logLevel Log level to convert
 *
 * \sa LogLevel
 * \sa levelFromString()
 */
QString Logger::levelToString(Logger::LogLevel logLevel)
{
  switch (logLevel)
  {
    case Trace:
      return QLatin1String("Trace");
    case Debug:
      return QLatin1String("Debug");
    case Info:
      return QLatin1String("Info");
    case Warning:
      return QLatin1String("Warning");
    case Error:
      return QLatin1String("Error");
    case Fatal:
      return QLatin1String("Fatal");
  }

  return QString();
}


//! Converts the LogLevel string representation to enum value
/**
 * Comparation of the strings is case independent. If the log level string provided cannot be understood
 * Logger::Debug is returned.
 *
 * \param s String to be decoded
 *
 * \sa LogLevel
 * \sa levelToString()
 */
Logger::LogLevel Logger::levelFromString(const QString& s)
{
  QString str = s.trimmed().toLower();

  LogLevel result = Debug;

  if (str == QLatin1String("trace"))
    result = Trace;
  else if (str == QLatin1String("debug"))
    result = Debug;
  else if (str == QLatin1String("info"))
    result = Info;
  else if (str == QLatin1String("warning"))
    result = Warning;
  else if (str == QLatin1String("error"))
    result = Error;
  else if (str == QLatin1String("fatal"))
    result = Fatal;

  return result;
}


//! Returns the global instance of Logger
/**
 * In a most cases you shouldn't use this function directly. Consider using [logger](@ref logger) macro instead.
 *
 * \sa logger
 */
Logger* Logger::globalInstance()
{
  Logger* result = 0;
  {
    QReadLocker locker(&LoggerPrivate::globalInstanceLock);
    result = LoggerPrivate::globalInstance;
  }

  if (!result)
  {
    QWriteLocker locker(&LoggerPrivate::globalInstanceLock);
    LoggerPrivate::globalInstance = new Logger;

#if QT_VERSION >= 0x050000
    qInstallMessageHandler(qtLoggerMessageHandler);
#else
    qInstallMsgHandler(qtLoggerMessageHandler);
#endif
    qAddPostRoutine(cleanupLoggerGlobalInstance);
    result = LoggerPrivate::globalInstance;
  }

  return result;
}


//! Registers the appender to write the log records to
/**
 * On the log writing call (using one of the macros or the write() function) Logger traverses through the list of
 * the appenders and writes a log records to the each of them. Please, look through the AbstractAppender
 * documentation to understand the concept of appenders.
 *
 * If no appenders was added to Logger, it falls back to logging into the \c std::cerr STL stream.
 *
 * \param appender Appender to register in the Logger
 *
 * \note Logger takes ownership on the appender and it will delete it on the application exit. According to this,
 *       appenders must be created on heap to prevent double destruction of the appender.
 *
 * \sa registerCategoryAppender
 * \sa AbstractAppender
 */
void Logger::registerAppender(AbstractAppender* appender)
{
  Q_D(Logger);

  QMutexLocker locker(&d->loggerMutex);

  if (!d->appenders.contains(appender))
    d->appenders.append(appender);
  else
    std::cerr << "Trying to register appender that was already registered" << std::endl;
}

//! Registers the appender to write the log records to the specific category
/**
 * Calling this method, you can link some appender with the named category.
 * On the log writing call to the specific category (calling write() with category parameter directly,
 * writing to the default category, or using special dCDebug(), dCWarning() etc. macros),
 * Logger writes the log message only to the list of registered category appenders.
 *
 * You can call logToGlobalInstance() to pass all category log messages to the global logger instance appenders
 * (registered using registerAppender()).
 * If no category appenders with specific name was registered to the Logger,
 * it falls back to logging into the \c std::cerr STL stream, both with simple warning message.
 *
 * \param category Category name
 * \param appender Appender to register in the Logger
 *
 * \note Logger takes ownership on the appender and it will delete it on the application exit. According to this,
 *       appenders must be created on heap to prevent double destruction of the appender.
 *
 * \sa registerAppender
 * \sa dCTrace(), dCDebug(), dCInfo(), dCWarning(), dCError(), dCFatal()
 * \sa dCategory(), dGlobalCategory()
 * \sa logToGlobalInstance
 * \sa setDefaultCategory
 */
void Logger::registerCategoryAppender(const QString& category, AbstractAppender* appender)
{
  Q_D(Logger);

  QMutexLocker locker(&d->loggerMutex);

  if (!d->categoryAppenders.values().contains(appender))
    d->categoryAppenders.insert(category, appender);
  else
    std::cerr << "Trying to register appender that was already registered" << std::endl;
}

//! Sets default logging category
/**
 * All log messages to this category appenders will also be written to general logger instance appenders (registered
 * using [registerAppender](@ref registerAppender) method), and vice versa.
 * In particular, any calls to the dDebug() macro will be treated as category logging,
 * so you needn't to specify category name using dCDebug() macro.
 *
 * To unset the default category, pass a null string as a parameter.
 *
 * \param category Category name
 *
 * \note "category" format marker will be set to the category name for all of these messages
 * (see [AbstractStringAppender::setFormat](@ref AbstractStringAppender::setFormat)).
 *
 * \sa defaultCategory()
 * \sa registerCategoryAppender()
 * \sa logToGlobalInstance()
 */
void Logger::setDefaultCategory(const QString& category)
{
  Q_D(Logger);

  QMutexLocker locker(&d->loggerMutex);

  d->defaultCategory = category;
}

//! Returns default logging category name
/**
 * \sa setDefaultCategory
 */
QString Logger::defaultCategory() const
{
  Q_D(const Logger);
  return d->defaultCategory;
}

//! Links some logging category with the global logger instance appenders.
/**
 * If set to true, all log messages to the specified category appenders will also be written to the global logger instance appenders,
 * registered using registerAppender().
 *
 * By default, all messages to the specific category are written only to the specific category appenders
 * (registered using registerCategoryAppender()).
 *
 * \param category Category name
 * \param logToGlobal Link or onlink the category from global logger instance appender
 *
 * \sa globalInstance
 * \sa registerAppender
 * \sa registerCategoryAppender
 */
void Logger::logToGlobalInstance(const QString& category, bool logToGlobal)
{
  Q_D(Logger);

  if (this == globalInstance())
  {
    QMutexLocker locker(&d->loggerMutex);
    d->categories.insert(category, logToGlobal);
  }
  else
  {
    globalInstance()->logToGlobalInstance(category, logToGlobal);
  }
}


void Logger::write(const QDateTime& timeStamp, LogLevel logLevel, const char* file, int line, const char* function, const char* category,
                   const QString& message, bool fromLocalInstance)
{
  Q_D(Logger);

  QMutexLocker locker(&d->loggerMutex);

  QString logCategory = QString::fromLatin1(category);
  if (logCategory.isNull() && !d->defaultCategory.isNull())
    logCategory = d->defaultCategory;

  bool wasWritten = false;
  bool isGlobalInstance = this == globalInstance();
  bool linkedToGlobal = isGlobalInstance && d->categories.value(logCategory, false);

  if (!logCategory.isNull())
  {
    QList<AbstractAppender*> appenders = d->categoryAppenders.values(logCategory);
    if (appenders.length() == 0)
    {
      if (logCategory != d->defaultCategory && !linkedToGlobal && !fromLocalInstance)
        std::cerr << "No appenders assotiated with category " << qPrintable(logCategory) << std::endl;
    }
    else
    {
      foreach (AbstractAppender* appender, appenders)
        appender->write(timeStamp, logLevel, file, line, function, logCategory, message);
      wasWritten = true;
    }
  }

  // the default category is linked to the main logger appenders
  // global logger instance also writes all linked categories to the main appenders
  if (logCategory.isNull() || logCategory == d->defaultCategory || linkedToGlobal)
  {
    if (!d->appenders.isEmpty())
    {
      foreach (AbstractAppender* appender, d->appenders)
        appender->write(timeStamp, logLevel, file, line, function, logCategory, message);
      wasWritten = true;
    }
    else
    {
      static bool noAppendersWarningShown = false;
      if (!noAppendersWarningShown)
      {
#if defined(Q_OS_ANDROID)
        __android_ write(ANDROID_LOG_WARN, "Logger", "No appenders registered with logger");
#else
        std::cerr << "No appenders registered with logger" << std::endl;
#endif
        noAppendersWarningShown = true;
      }
    }
  }

  // local logger instances send category messages to the global instance
  if (!logCategory.isNull() && !isGlobalInstance)
    globalInstance()->write(timeStamp, logLevel, file, line, function, logCategory.toLatin1(), message, true);

  if (!wasWritten && !fromLocalInstance)
  {
    // Fallback
#if defined(Q_OS_ANDROID)
    QString result = QString(QLatin1String("<%2> %3")).arg(AbstractStringAppender::stripFunctionName(function)).arg(message);
    __android_log_write(AndroidAppender::androidLogPriority(logLevel), "Logger", qPrintable(result));
#else
    QString result = QString(QLatin1String("[%1] <%2> %3")).arg(levelToString(logLevel), -7)
                     .arg(AbstractStringAppender::stripFunctionName(function)).arg(message);
    std::cerr << qPrintable(result) << std::endl;
#endif
  }

  if (logLevel == Logger::Fatal)
    abort();
}


//! Writes the log record
/**
 * Writes the log records with the supplied arguments to all the registered appenders.
 *
 * \note It is not recommended to call this function directly. Instead of this you can just call one of the macros
 *       (dTrace, dTrac, dInfo, dWarning, dError, dFatal) that will supply all the needed
 *       information to this function.
 *
 * \param timeStamp - the time stamp of the record
 * \param logLevel - the log level of the record
 * \param file - the name of the source file that requested the log record
 * \param line - the line of the code of source file that requested the log record
 * \param function - name of the function that requested the log record
 * \param category - logging category (0 for default category)
 * \param message - log message
 *
 * \note Recording of the log record using the Logger::Fatal log level will lead to calling the STL abort()
 *       function, which will interrupt the running of your software and begin the writing of the core dump.
 *
 * \sa LogLevel
 * \sa dTrace, dDebug, dInfo, dWarning, dError, dFatal
 * \sa AbstractAppender
 */
void Logger::write(const QDateTime& timeStamp, LogLevel logLevel, const char* file, int line, const char* function, const char* category,
                   const QString& message)
{
  write(timeStamp, logLevel, file, line, function, category, message, /* fromLocalInstance = */ false);
}

/**
 * This is the overloaded function provided for the convinience. It behaves similar to the above function.
 *
 * This function uses the current timestamp obtained with \c QDateTime::currentDateTime().
 *
 * \sa write()
 */
void Logger::write(LogLevel logLevel, const char* file, int line, const char* function, const char* category,
                   const QString& message)
{
  write(QDateTime::currentDateTime(), logLevel, file, line, function, category, message);
}


/**
 * This is the overloaded function provided for the convinience. It behaves similar to the above function.
 *
 * This function doesn't accept any log message as argument. It returns the \c QDebug object that can be written
 * using the stream functions. For example, you may like to write:
 * \code
 * dDebug() << "This is the size" << size << "of the element" << elementName;
 * \endcode
 * instead of writing
 * \code
 * dDebug(QString(QLatin1String("This is the size %1x%2 of the element %3"))
 *           .arg(size.x()).arg(size.y()).arg(elementName));
 * \endcode
 *
 * Please consider reading the Qt Reference Documentation for the description of the QDebug class usage syntax.
 *
 * \note This overload is definitely more pleasant to use than the first write() overload, but it behaves definitely
 *       slower than all the above overloads.
 *
 * \sa write()
 */
QDebug Logger::write(LogLevel logLevel, const char* file, int line, const char* function, const char* category)
{
  Q_D(Logger);

  d->logDevice->lock(logLevel, file, line, function, category);
  return QDebug(d->logDevice);
}


//! Writes the assertion
/**
 * This function writes the assertion record using the write() function.
 *
 * The assertion record is always written using the Logger::Fatal log level which leads to the abortation of the
 * program and generation of the core dump (if supported).
 *
 * The message written to the appenders will be identical to the \c condition argument prefixed with the
 * <tt>ASSERT:</tt> notification.
 *
 * \note It is not recommended to call this function directly. Instead of this you can just call the LOG_ASSERT
 *       macro that will supply all the needed information to this function.
 *
 * \sa LOG_ASSERT
 * \sa write()
 */
void Logger::writeAssert(const char* file, int line, const char* function, const char* condition)
{
  write(Logger::Fatal, file, line, function, 0, QString("ASSERT: \"%1\"").arg(condition));
}


Logger* loggerInstance()
{
  return Logger::globalInstance();
}



void LoggerTimingHelper::start(const char* msg, ...)
{
  va_list va;
  va_start(va, msg);
  m_block = QString().vsprintf(msg, va);
  va_end(va);

  m_time.start();
}


void LoggerTimingHelper::start(const QString& block)
{
  m_block = block;
  m_time.start();
}

LoggerTimingHelper::~LoggerTimingHelper()
{
  QString message;
  if (m_block.isEmpty())
    message = QString(QLatin1String("Function %1 finished in ")).arg(AbstractStringAppender::stripFunctionName(m_function));
  else
    message = QString(QLatin1String("\"%1\" finished in ")).arg(m_block);

  int elapsed = m_time.elapsed();
  if (elapsed >= 10000)
    message += QString(QLatin1String("%1 s.")).arg(elapsed / 1000);
  else
    message += QString(QLatin1String("%1 ms.")).arg(elapsed);

  m_logger->write(m_logLevel, m_file, m_line, m_function, 0, message);
}


void CuteMessageLogger::write(const char* msg, ...) const
{
  va_list va;
  va_start(va, msg);
  m_l->write(m_level, m_file, m_line, m_function, m_category, QString().vsprintf(msg, va));
  va_end(va);
}


void CuteMessageLogger::write(const QString& msg) const
{
  m_l->write(m_level, m_file, m_line, m_function, m_category, msg);
}


QDebug CuteMessageLogger::write() const
{
  return m_l->write(m_level, m_file, m_line, m_function, m_category);
}

DUTIL_END_NAMESPACE
