/*
 * Copyright (C) 2015 ~ 2018 Wuhan Deepin Technology Co., Ltd.
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

#include "Cmd.h"

#include "../Common/Result.h"
#include "../FileSystem/FileSystem.h"

#include <QDebug>
#include <QString>
#include <QTextStream>
#include <QFile>
#include <QProcess>

namespace XSys
{


static Result runApp(const QString &execPath, const QString &execParam, const QString &execPipeIn = "")
{
//   QString outPipePath = FS::TmpFilePath("pipeOut");

    QProcess app;
    app.setStandardInputFile(execPipeIn);
//    app.setStandardOutputFile(outPipePath);
//    app.setStandardErrorFile(outPipePath);
    app.start("\"" + execPath + "\"" + " " + execParam);
    if (!app.waitForStarted()) {
        qWarning() << "Cmd Exec Failed:" << app.errorString();
        return Result(Result::Faiiled, app.errorString(), "", app.program());
    }

    if (!app.waitForFinished(-1)) {
        qWarning() << "waitForFinished Failed:" << app.errorString();
        return Result(Result::Faiiled, app.errorString(), "", app.program());
    }

    auto standardError = app.readAllStandardError();

    if (QProcess::NormalExit != app.exitStatus()) {
        qWarning() << "exitStatus error:" << app.exitStatus() << standardError << app.program();
        return Result(Result::Faiiled, standardError, "", app.program());
    }

    if (0 != app.exitCode()) {
        qWarning() << "exitCode error:" << app.exitCode() << standardError << app.program();
        return Result(Result::Faiiled, standardError, "", app.program());
    }

    Result rest(Result::Success, standardError, app.readAllStandardOutput());
    return rest;
}

Result SynExec(const QString &exec, const QString &param, const QString &execPipeIn)
{
    Result ret = runApp(exec, param, execPipeIn);
    qDebug() << "call:" << exec + " " + param << "with:" << execPipeIn ;
    qDebug() << "resut:" << ret.isSuccess() << ret.errmsg();
    return ret;
}

static Result runApp(const QString &execPath, const QStringList &args)
{
//   QString outPipePath = FS::TmpFilePath("pipeOut");
    QProcess app;
    app.setProgram(execPath);
    app.setArguments(args);
    app.start();

    if (!app.waitForStarted()) {
        qWarning() << "Cmd Exec Failed:" << app.errorString();
        return Result(Result::Faiiled, app.errorString(), "", app.program());
    }

    if (!app.waitForFinished(-1)) {
        qWarning() << "waitForFinished Failed:" << app.errorString();
        return Result(Result::Faiiled, app.errorString(), "", app.program());
    }

    auto standardError = app.readAllStandardError();

    if (QProcess::NormalExit != app.exitStatus()) {
        qWarning() << "exitStatus error:" << app.exitStatus() << standardError << app.program();
        return Result(Result::Faiiled, standardError, "", app.program());
    }

    if (0 != app.exitCode()) {
        qWarning() << "exitCode error:" << app.exitCode() << standardError << app.program();
        return Result(Result::Faiiled, standardError, "", app.program());
    }

    Result rest(Result::Success, standardError, app.readAllStandardOutput());
    return rest;
}

Result SynExec(const QString &exec, const QStringList &args)
{
    Result ret = runApp(exec, args);
    qDebug() << "call:" << exec << args ;
    qDebug() << "resut:" << ret.isSuccess() << ret.errmsg();
    return ret;
}


}
