// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "logger.h"

#include <QLoggingCategory>

#ifdef DTKCORE_CLASS_DConfigFile
#include <DConfig>
DCORE_USE_NAMESPACE
#endif

MLogger::MLogger(QObject *parent)
    : QObject(parent), m_rules("")
#ifdef DTKCORE_CLASS_DConfigFile
    , m_config(nullptr)
#endif
{
    QByteArray logRules = qgetenv("QT_LOGGING_RULES");
    // 置空环境变量前一定不要有任何日志打印，否则设置不会生效
    qputenv("QT_LOGGING_RULES", "");

    // 未显式指定环境变量规则时，默认静默未分类日志（Qt 框架/vendor 库的 debug），
    // 本应用分类由 DConfig 规则放行（规则后置优先生效）
    if (logRules.isEmpty())
        logRules = "*.debug=false";

    // set env
    m_rules = logRules;

#ifdef DTKCORE_CLASS_DConfigFile
    // set dconfig
    m_config = DConfig::create("org.deepin.boot-maker", "org.deepin.boot-maker");
    logRules = m_config->value("log_rules").toByteArray();
    appendRules(logRules);
    setRules(m_rules);

    // 监听 dconfig 变更，实现运行中动态调整（调试模式切换日志级别）
    connect(m_config, &DConfig::valueChanged, this, [this](const QString &key) {
        if (key == "log_rules") {
            setRules(m_config->value(key).toByteArray());
        }
    });
#else
    setRules(m_rules);
#endif
}

MLogger::~MLogger()
{
#ifdef DTKCORE_CLASS_DConfigFile
    if (m_config) {
        m_config->deleteLater();
    }
#endif
}

void MLogger::setRules(const QString &rules)
{
    auto tmpRules = rules;
    m_rules = tmpRules.replace(";", "\n");   // Qt 限制：setFilterRules 分隔符必须是 \n
    QLoggingCategory::setFilterRules(m_rules);
}

void MLogger::appendRules(const QString &rules)
{
    QString tmpRules = rules;
    tmpRules = tmpRules.replace(";", "\n");
    auto tmplist = tmpRules.split('\n');
    for (int i = 0; i < tmplist.count(); i++)
        if (m_rules.contains(tmplist.at(i))) {
            tmplist.removeAt(i);
            i--;
        }
    if (tmplist.isEmpty())
        return;
    m_rules.isEmpty() ? m_rules = tmplist.join("\n")
                      : m_rules += "\n" + tmplist.join("\n");
}
