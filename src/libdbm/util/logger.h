// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#ifndef BOOTMAKER_LOGGER_H
#define BOOTMAKER_LOGGER_H

#include <QObject>
#include <dtkcore_global.h>

#ifdef DTKCORE_CLASS_DConfigFile
DCORE_BEGIN_NAMESPACE
class DConfig;
DCORE_END_NAMESPACE
#endif

// 动态日志规则管理：从 DConfig(org.deepin.boot-maker) 读取 log_rules 并监听变更，
// 实现"调试模式"运行中动态调整日志级别（参考 deepin-reader 的 MLogger 实现）。
// 注意：必须在任何日志打印之前实例化（置空 QT_LOGGING_RULES 才能生效），
// 且要在 Application 实例化之前（QApplication 内部可能提前打印日志）。
class MLogger : public QObject
{
    Q_OBJECT
public:
    explicit MLogger(QObject *parent = nullptr);
    ~MLogger();

    inline QString rules() const { return m_rules; }
    void setRules(const QString &rules);

private:
    void appendRules(const QString &rules);

private:
    QString m_rules;
#ifdef DTKCORE_CLASS_DConfigFile
    Dtk::Core::DConfig *m_config;
#endif
};

#endif // BOOTMAKER_LOGGER_H
