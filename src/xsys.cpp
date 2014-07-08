#include "xsys.h"

#include <QtCore>

namespace XAPI {

#ifdef Q_OS_WIN32

#include <windows.h>
#include <shellapi.h>
QString RunApp(const QString &execPath, const QString &execParam) {
    // TODO: rewrite by createprocess
    SHELLEXECUTEINFO ShExecInfo;
    memset(&ShExecInfo,0,sizeof(SHELLEXECUTEINFO));
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd = NULL;
    if (QSysInfo::WindowsVersion == QSysInfo::WV_NT || QSysInfo::WindowsVersion == QSysInfo::WV_2000 || QSysInfo::WindowsVersion == QSysInfo::WV_XP || QSysInfo::WindowsVersion == QSysInfo::WV_2003 )
    {
        ShExecInfo.lpVerb = NULL;
    }
    else
    {
        ShExecInfo.lpVerb = L"runas";
    }
    ShExecInfo.lpFile = LPWSTR(execPath.utf16());
    ShExecInfo.lpParameters = LPWSTR(execParam.utf16());
    ShExecInfo.lpDirectory = NULL;
    ShExecInfo.nShow = SW_HIDE;
    ShExecInfo.hInstApp = NULL;
    ShellExecuteEx(&ShExecInfo);
    WaitForSingleObject(ShExecInfo.hProcess,INFINITE);
    return "";
}
#endif

#ifdef Q_OS_UNIX
QString RunApp(const QString &execPath, const QString &execParam) {
	QProcess app;
    QString cmd = execPath + " " + execParam;
    app.start(cmd);
    qDebug()<<"RunAPP: "<<cmd;
    app.waitForFinished(-1);
    return QString(app.readAll());
}
#endif

}

class Execer: public QObject {
public:
    void run();
    QString ExecPath;
    QString Param;
    QString Ret;

};


void Execer::run(){
    Ret = XAPI::RunApp(ExecPath, Param);
    qDebug()<<"Exec: "<<ExecPath<<endl
            <<"Params: "<<Param<<endl
            <<"Output: "<<Ret<<endl;
}

XSys::XSys(QObject *parent) :
    QObject(parent) {
}

QString XSys::SynExec(const QString &exec, const QString &param) {
    Execer execer;
    execer.ExecPath = exec;
    execer.Param = param;
    execer.run();
    return execer.Ret;
}

QString XSys::RandString(const QString &str) {
    QString seedStr = str + QTime::currentTime().toString(Qt::SystemLocaleLongDate) + QString("%1").arg(qrand());
    return QString("").append(QCryptographicHash::hash(seedStr.toLatin1(), QCryptographicHash::Md5).toHex());
}

QString XSys::TmpFilePath(const QString &filename) {
    QString tmpDir = QStandardPaths::standardLocations(QStandardPaths::TempLocation)[0];
    QString ext =  + "." + filename.split(".").last();
    if ("." == ext ) {
        ext = "";
    }
    QString newFilename = RandString(filename);
    qDebug()<<"New tmpFilename"<<newFilename;
    return QDir::toNativeSeparators(QString( tmpDir + "/"
            + newFilename + ext));
}

QString XSys::InsertTmpFile(const QByteArray &data) {
    QString filename = TmpFilePath();
    QFile tmpFile(filename);
    tmpFile.open(QIODevice::WriteOnly);
    tmpFile.write(data);
    tmpFile.close();
    qDebug()<<"Create Tmp File: "<<filename;
    return filename;
}

void XSys::RmFile(QFile &fn)
{
    if (!fn.exists()) return;
    fn.setPermissions(QFile::WriteUser);
    fn.remove();
#ifdef Q_OS_UNIX
    SynExec("sync", "");
#endif
}


void XSys::RmFile(const QString &filename)
{
    QFile file(filename);
    RmFile(file);

}

void XSys::CpFile(const QString &srcName, const QString &desName) {
    QFile srcFile(srcName);
    QFile desFile(desName);
    srcFile.open(QIODevice::ReadOnly);
    desFile.open(QIODevice::WriteOnly);
    desFile.write(srcFile.readAll());
    srcFile.close();
    desFile.close();
}
