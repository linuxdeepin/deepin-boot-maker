// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#ifndef DDLOG_H
#define DDLOG_H

#include <QDebug>
#include <QLoggingCategory>

// 全项目统一的日志分类（启动盘制作工具），便于调试模式(DConfig)按应用统一过滤：
// 主程序(GUI)：org.deepin.boot-maker
// D-Bus 服务：org.deepin.boot-maker.service
// libdbm 按子模块进一步细分，均以 org.deepin.boot-maker 为前缀，
// 调试脚本用 *boot-maker* 通配规则即可整体开关
inline Q_LOGGING_CATEGORY(appLog, "org.deepin.boot-maker")

// 模块分离：com.deepin.bootmaker D-Bus 服务进程
inline Q_LOGGING_CATEGORY(serviceLog, "org.deepin.boot-maker.service")

// 模块分离：libdbm 后端（bootmaker/bmhandler/bminterface 等）
inline Q_LOGGING_CATEGORY(backendLog, "org.deepin.boot-maker.backend")

// 模块分离：libdbm 安装器（installer）
inline Q_LOGGING_CATEGORY(installerLog, "org.deepin.boot-maker.installer")

// 模块分离：libdbm 工具类（util）
inline Q_LOGGING_CATEGORY(utilLog, "org.deepin.boot-maker.util")

#endif   // DDLOG_H
