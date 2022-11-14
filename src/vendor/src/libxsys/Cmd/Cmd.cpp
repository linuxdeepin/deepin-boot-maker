// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "Cmd.h"

#include "../Common/Result.h"
#include "../FileSystem/FileSystem.h"

#include <QDebug>
#include <QString>
#include <QTextStream>
#include <QFile>
#include <QProcess>

namespace XSys {


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
        return Result(Result::Failed, app.errorString(), "", app.program());
    }

    if (!app.waitForFinished(-1)) {
        qWarning() << "waitForFinished Failed:" << app.errorString();
        return Result(Result::Failed, app.errorString(), "", app.program());
    }

    auto standardError = app.readAllStandardError();

    if (QProcess::NormalExit != app.exitStatus()) {
        qWarning() << "exitStatus error:" << app.exitStatus() << standardError << app.program();
        return Result(Result::Failed, standardError, "", app.program());
    }

    if (0 != app.exitCode()) {
        //qWarning() << "exitCode error:" << app.exitCode() << standardError << app.program();
        return Result(Result::Failed, standardError, "", app.program());
    }

    Result rest(Result::Success, standardError, app.readAllStandardOutput());
    return rest;
}

Result SynExec(const QString &exec, const QString &param, const QString &execPipeIn)
{
    Result ret = runApp(exec, param, execPipeIn);
    qInfo() << "call:" << exec + " " + param << "with:" << execPipeIn ;
    qInfo() << "resut:" << ret.isSuccess();
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
        return Result(Result::Failed, app.errorString(), "", app.program());
    }

    if (!app.waitForFinished(-1)) {
        qWarning() << "waitForFinished Failed:" << app.errorString();
        return Result(Result::Failed, app.errorString(), "", app.program());
    }

    auto standardError = app.readAllStandardError();

    if (QProcess::NormalExit != app.exitStatus()) {
        qWarning() << "exitStatus error:" << app.exitStatus() << standardError << app.program();
        return Result(Result::Failed, standardError, "", app.program());
    }

    if (0 != app.exitCode()) {
        //qWarning() << "exitCode error:" << app.exitCode() << standardError << app.program();
        return Result(Result::Failed, standardError, "", app.program());
    }

    Result rest(Result::Success, standardError, app.readAllStandardOutput());
    return rest;
}

Result SynExec(const QString &exec, const QStringList &args)
{
    Result ret = runApp(exec, args);

    if (exec.trimmed().toLower() != QString("isoinfo")) {
        qInfo() << "call:" << exec << args ;
        qInfo() << "resut:" << ret.isSuccess() << ret.errmsg();
    }

    return ret;
}


}
