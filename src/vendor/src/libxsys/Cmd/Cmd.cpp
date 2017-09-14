/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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
        qWarning() << "Cmd Exec Failed:" << app.errorString();
        return Result(Result::Faiiled, app.errorString(), "", app.program());
    }

    if (QProcess::NormalExit != app.exitStatus()) {
        qWarning() << "Cmd Exec Failed:" << app.readAllStandardError();
        return Result(Result::Faiiled, app.readAllStandardError(), "", app.program());
    }

    if (0 != app.exitCode()) {
        qWarning() << "Cmd Exec Failed:" << app.readAllStandardError() << app.program();
        return Result(Result::Faiiled, app.readAllStandardError(), "", app.program());
    }

    Result rest(Result::Success, app.readAllStandardError(), app.readAllStandardOutput());
    return rest;
}

Result SynExec(const QString &exec, const QString &param, const QString &execPipeIn)
{
    Result ret = runApp(exec, param, execPipeIn);
    qDebug() << exec + " " + param
             << ret.isSuccess() << ret.errmsg()
             << execPipeIn ;
    return ret;
}

}
