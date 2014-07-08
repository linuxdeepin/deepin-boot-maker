#include "xsys.h"

#include <QtCore>

namespace XAPI {

#ifdef Q_OS_WIN32

#include <windows.h>
#include <shellapi.h>
QString RunApp(const QString& execPath, const QString& execParam) {
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

QString XSys::SynExec(const QString& exec, const QString& param) {
    Execer execer;
    execer.ExecPath = exec;
    execer.Param = param;
    execer.run();
    return execer.Ret;
}

QString XSys::TmpFilePath(const QString& filename) {
    QString tmpDir = QStandardPaths::standardLocations(QStandardPaths::TempLocation)[0];
    QString randStr = QTime::currentTime().toString(Qt::SystemLocaleLongDate) + QString("%1").arg(qrand());
    QString newFilename;
    newFilename.append(QCryptographicHash::hash((randStr + filename).toLatin1(), QCryptographicHash::Md5).toHex());
    qDebug()<<"New tmpFilename"<<newFilename;
    return QDir::toNativeSeparators(QString( tmpDir + "\\"
            + newFilename + "."
            + filename.split(".").last()));
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


void XSys::RmFile(const QString& filename)
{
    RmFile(QFile(filename));
}
