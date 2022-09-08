// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QtCore>

#include <Logger.h>
#include <ConsoleAppender.h>
#include <RollingFileAppender.h>

class DBMLogManager
{
public:
    static void setSystemLog(bool sys);
    static void registerConsoleAppender();
    static void registerFileAppender();
    static QString getlogFilePath();
    static void setLogFormat(const QString& format);

signals:

public slots:

private:
    bool systemLog = false;
    QString m_format;
    QString m_logPath;
    Dtk::Core::ConsoleAppender* m_consoleAppender;
    Dtk::Core::RollingFileAppender* m_rollingFileAppender;

    void initConsoleAppender();
    void initRollingFileAppender();
    QString joinPath(const QString& path, const QString& fileName);

    inline static DBMLogManager* instance(){
        static DBMLogManager instance;
        return &instance;
    }
    explicit DBMLogManager();
    ~DBMLogManager();
    DBMLogManager(const DBMLogManager &);
    DBMLogManager & operator = (const DBMLogManager &);
};

