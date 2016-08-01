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
#ifndef LOGGER_H
#define LOGGER_H

// Qt
#include <QString>
#include <QDebug>
#include <QDateTime>

// Local
#include "CuteLogger_global.h"

DUTIL_BEGIN_NAMESPACE

class AbstractAppender;
class Logger;
CUTELOGGERSHARED_EXPORT Logger* loggerInstance();
#define logger loggerInstance()


#define dTrace            CuteMessageLogger(loggerInstance(), Logger::Trace,   __FILE__, __LINE__, Q_FUNC_INFO).write
#define dDebug            CuteMessageLogger(loggerInstance(), Logger::Debug,   __FILE__, __LINE__, Q_FUNC_INFO).write
#define dInfo             CuteMessageLogger(loggerInstance(), Logger::Info,    __FILE__, __LINE__, Q_FUNC_INFO).write
#define dWarning          CuteMessageLogger(loggerInstance(), Logger::Warning, __FILE__, __LINE__, Q_FUNC_INFO).write
#define dError            CuteMessageLogger(loggerInstance(), Logger::Error,   __FILE__, __LINE__, Q_FUNC_INFO).write
#define dFatal            CuteMessageLogger(loggerInstance(), Logger::Fatal,   __FILE__, __LINE__, Q_FUNC_INFO).write

#define dCTrace(category)   CuteMessageLogger(loggerInstance(), Logger::Trace,   __FILE__, __LINE__, Q_FUNC_INFO, category).write()
#define dCDebug(category)   CuteMessageLogger(loggerInstance(), Logger::Debug,   __FILE__, __LINE__, Q_FUNC_INFO, category).write()
#define dCInfo(category)    CuteMessageLogger(loggerInstance(), Logger::Info,    __FILE__, __LINE__, Q_FUNC_INFO, category).write()
#define dCWarning(category) CuteMessageLogger(loggerInstance(), Logger::Warning, __FILE__, __LINE__, Q_FUNC_INFO, category).write()
#define dCError(category)   CuteMessageLogger(loggerInstance(), Logger::Error,   __FILE__, __LINE__, Q_FUNC_INFO, category).write()
#define dCFatal(category)   CuteMessageLogger(loggerInstance(), Logger::Fatal,   __FILE__, __LINE__, Q_FUNC_INFO, category).write()

#define dTraceTime  LoggerTimingHelper loggerTimingHelper(loggerInstance(), Logger::Trace, __FILE__, __LINE__, Q_FUNC_INFO); loggerTimingHelper.start
#define dDebugTime  LoggerTimingHelper loggerTimingHelper(loggerInstance(), Logger::Debug, __FILE__, __LINE__, Q_FUNC_INFO); loggerTimingHelper.start
#define dInfoTime   LoggerTimingHelper loggerTimingHelper(loggerInstance(), Logger::Info,  __FILE__, __LINE__, Q_FUNC_INFO); loggerTimingHelper.start

#define dAssert(cond)        ((!(cond)) ? loggerInstance()->writeAssert(__FILE__, __LINE__, Q_FUNC_INFO, #cond) : qt_noop())
#define dAssertX(cond, msg)  ((!(cond)) ? loggerInstance()->writeAssert(__FILE__, __LINE__, Q_FUNC_INFO, msg) : qt_noop())

#define dCategory(category) \
  private:\
    Logger* loggerInstance()\
    {\
      static Logger customLoggerInstance(category);\
      return &customLoggerInstance;\
    }\

#define dGlobalCategory(category) \
  private:\
    Logger* loggerInstance()\
    {\
      static Logger customLoggerInstance(category);\
      customLoggerInstance.logToGlobalInstance(category, true);\
      return &customLoggerInstance;\
    }\


class LoggerPrivate;
class CUTELOGGERSHARED_EXPORT Logger
{
  Q_DISABLE_COPY(Logger)

  public:
    Logger();
    Logger(const QString& defaultCategory);
    ~Logger();

    //! Describes the possible severity levels of the log records
    enum LogLevel
    {
      Trace,   //!< Trace level. Can be used for mostly unneeded records used for internal code tracing.
      Debug,   //!< Debug level. Useful for non-necessary records used for the debugging of the software.
      Info,    //!< Info level. Can be used for informational records, which may be interesting for not only developers.
      Warning, //!< Warning. May be used to log some non-fatal warnings detected by your application.
      Error,   //!< Error. May be used for a big problems making your application work wrong but not crashing.
      Fatal    //!< Fatal. Used for unrecoverable errors, crashes the application right after the log record is written.
    };

    static QString levelToString(LogLevel logLevel);
    static LogLevel levelFromString(const QString& s);

    static Logger* globalInstance();

    void registerAppender(AbstractAppender* appender);
    void registerCategoryAppender(const QString& category, AbstractAppender* appender);

    void logToGlobalInstance(const QString& category, bool logToGlobal = false);

    void setDefaultCategory(const QString& category);
    QString defaultCategory() const;

    void write(const QDateTime& timeStamp, LogLevel logLevel, const char* file, int line, const char* function, const char* category,
               const QString& message);
    void write(LogLevel logLevel, const char* file, int line, const char* function, const char* category, const QString& message);
    QDebug write(LogLevel logLevel, const char* file, int line, const char* function, const char* category);

    void writeAssert(const char* file, int line, const char* function, const char* condition);

  private:
    void write(const QDateTime& timeStamp, LogLevel logLevel, const char* file, int line, const char* function, const char* category,
               const QString& message, bool fromLocalInstance);
    Q_DECLARE_PRIVATE(Logger)
    LoggerPrivate* d_ptr;
};


class CUTELOGGERSHARED_EXPORT CuteMessageLogger
{
  Q_DISABLE_COPY(CuteMessageLogger)

  public:
    Q_DECL_CONSTEXPR CuteMessageLogger(Logger* l, Logger::LogLevel level, const char* file, int line, const char* function)
        : m_l(l),
          m_level(level),
          m_file(file),
          m_line(line),
          m_function(function),
          m_category(0)
    {}

    Q_DECL_CONSTEXPR CuteMessageLogger(Logger* l, Logger::LogLevel level, const char* file, int line, const char* function, const char* category)
        : m_l(l),
          m_level(level),
          m_file(file),
          m_line(line),
          m_function(function),
          m_category(category)
    {}

    void write(const char* msg, ...) const
#if defined(Q_CC_GNU) && !defined(__INSURE__)
#  if defined(Q_CC_MINGW) && !defined(Q_CC_CLANG)
    __attribute__ ((format (gnu_printf, 2, 3)))
#  else
    __attribute__ ((format (printf, 2, 3)))
#  endif
#endif
    ;

    void write(const QString& msg) const;

    QDebug write() const;

  private:
    Logger* m_l;
    Logger::LogLevel m_level;
    const char* m_file;
    int m_line;
    const char* m_function;
    const char* m_category;
};


class CUTELOGGERSHARED_EXPORT LoggerTimingHelper
{
  Q_DISABLE_COPY(LoggerTimingHelper)

  public:
    inline explicit LoggerTimingHelper(Logger* l, Logger::LogLevel logLevel, const char* file, int line,
                                       const char* function)
      : m_logger(l),
        m_logLevel(logLevel),
        m_file(file),
        m_line(line),
        m_function(function)
    {}

    void start(const char* msg, ...)
#if defined(Q_CC_GNU) && !defined(__INSURE__)
  #  if defined(Q_CC_MINGW) && !defined(Q_CC_CLANG)
    __attribute__ ((format (gnu_printf, 2, 3)))
  #  else
    __attribute__ ((format (printf, 2, 3)))
  #  endif
#endif
        ;

    void start(const QString& msg = QString());

    ~LoggerTimingHelper();

  private:
    Logger* m_logger;
    QTime m_time;
    Logger::LogLevel m_logLevel;
    const char* m_file;
    int m_line;
    const char* m_function;
    QString m_block;
};

DUTIL_END_NAMESPACE

#endif // LOGGER_H
