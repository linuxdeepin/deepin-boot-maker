#include "xsys.h"

#include <QtCore>
#include <QApplication>

namespace XAPI {

#ifdef Q_OS_WIN32

#include <windows.h>
#include <shellapi.h>
QString RunApp(const QString &execPath, const QString &execParam, const QString &execPipeIn="") {
    SECURITY_ATTRIBUTES sa={sizeof(sa),NULL,TRUE};
    SECURITY_ATTRIBUTES *psa=NULL;
    DWORD dwShareMode=FILE_SHARE_READ|FILE_SHARE_WRITE;
    OSVERSIONINFO osVersion={0};
    osVersion.dwOSVersionInfoSize=sizeof(osVersion);

    if(GetVersionEx(&osVersion)){
        if(osVersion.dwPlatformId==VER_PLATFORM_WIN32_NT){
        psa=&sa;
        dwShareMode|=FILE_SHARE_DELETE;
        }
    }

    QString outPipePath = XSys::TmpFilePath("pipeOut");
    HANDLE hConsoleCoutRedirect=CreateFile(
        LPWSTR(outPipePath.utf16()),
        GENERIC_WRITE,
        dwShareMode,
        psa,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
        );

    if(hConsoleCoutRedirect==INVALID_HANDLE_VALUE)
    {
        qWarning()<<"cout error"<<GetLastError();
    };

    QString inPipePath = XSys::TmpFilePath("pipeIn");
    QFile inFile(inPipePath);
    inFile.open(QIODevice::WriteOnly);
    inFile.write(execPipeIn.toLatin1());
    inFile.close();

    HANDLE hConsoleCinRedirect=CreateFile(
            LPWSTR(inPipePath.utf16()),
            GENERIC_READ,
            dwShareMode,
            psa,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL
            );

    STARTUPINFO s={sizeof(s)};
    s.dwFlags=STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;
    s.hStdOutput=hConsoleCoutRedirect;
    s.hStdInput=hConsoleCinRedirect;
    s.wShowWindow=SW_HIDE;
    s.lpReserved = NULL;
    s.lpDesktop = NULL;
    s.lpTitle = NULL;
    s.wShowWindow = SW_HIDE;
    s.cbReserved2 = NULL;
    s.lpReserved2 = NULL;
    PROCESS_INFORMATION pi={0};

    QString cmdline = execPath + " " + execParam;

    if(CreateProcess(NULL,LPWSTR(cmdline.utf16()),NULL,NULL,TRUE,NULL,NULL,NULL,&s,&pi))
    {
        WaitForSingleObject(pi.hProcess,INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        CloseHandle(hConsoleCoutRedirect);
        CloseHandle(hConsoleCinRedirect);
    }

    QFile outFile(outPipePath);
    outFile.open(QIODevice::ReadOnly);
    QString ret = outFile.readAll();
    outFile.close();

    inFile.remove();
    outFile.remove();

    return ret;
}
#endif

#ifdef Q_OS_UNIX
QString RunApp(const QString &execPath, const QString &execParam, const QString &execPipeIn="") {
    QProcess app;
    QString cmdline = execPath + " " + execParam;
    app.start(cmdline);
    if (!app.waitForStarted()) {
        qWarning()<<"Start app failed: "<<cmdline;
        return "";
    }

    app.write(execPipeIn.toLatin1());
    app.closeWriteChannel();

    if (!app.waitForFinished()) {
        qWarning()<<"App quit failed: "<<cmdline;
        return "";
    }

    return app.readAll();
}
#endif

}

class Execer: public QObject {
public:
    void run(const QString &execPipeIn="");
    QString ExecPath;
    QString Param;
    QString Ret;

};


void Execer::run(const QString &execPipeIn){
    Ret = XAPI::RunApp(ExecPath, Param, execPipeIn);
    qDebug()<<"Exec: "<<ExecPath
            <<"Params: "<<Param
            <<"Output: "<<Ret;
}

XSys::XSys(QObject *parent) :
    QObject(parent) {
}

QString XSys::SynExec(const QString &exec, const QString &param, const QString &execPipeIn) {
    Execer execer;
    execer.ExecPath = exec;
    execer.Param = param;
    execer.run(execPipeIn);
    return execer.Ret;
}

QString XSys::RandString(const QString &str) {
    QString seedStr = str + QTime::currentTime().toString(Qt::SystemLocaleLongDate) + QString("%1").arg(qrand());
    return QString("").append(QCryptographicHash::hash(seedStr.toLatin1(), QCryptographicHash::Md5).toHex());
}

QString XSys::TmpFilePath(const QString &filename) {
    QString tmpDir = QStandardPaths::standardLocations(QStandardPaths::TempLocation)[0];
    QString ext =  + "." + filename.split(".").last();
    if ("." == ext || !filename.contains(".") ) {
        ext = "";
    }
    QString newFilename = RandString(filename);
    qDebug()<<"New tmpFilename"<<newFilename;
    return QDir::toNativeSeparators(QString( tmpDir + "/"
            + newFilename + ext));
}

QString XSys::InsertTmpFile(const QString &fileurl) {
    QString filename = TmpFilePath(fileurl);
    QFile file(fileurl);
    file.open(QIODevice::ReadOnly);
    QString tmpPath = InsertFileData(filename, file.readAll());
    file.close();
    return tmpPath;
}

QString XSys::InsertFile(const QString &fileurl, const QString &fullpath) {
    QFile file(fileurl);
    file.open(QIODevice::ReadOnly);
    QString tmpPath = InsertFileData(fullpath, file.readAll());
    file.close();
    return tmpPath;
}

QString XSys::InsertFileData(const QString &filename, const QByteArray &data) {
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    file.write(data);
    file.close();
    qDebug()<<"Create File: "<<filename;
#ifdef Q_OS_UNIX
    XSys::SynExec("sync", "");
#endif
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

bool XSys::CpFile(const QString &srcName, const QString &desName) {
    bool ret = true;
    QFile srcFile(srcName);
    QFile desFile(desName);
    srcFile.open(QIODevice::ReadOnly);
    desFile.open(QIODevice::WriteOnly);
    QByteArray data = srcFile.readAll();
    qint64 writeBytes = desFile.write(data);
    if (writeBytes != data.size()) {
        qWarning()<<"Copy File Failed, "<<srcName<<" to "<<desName;
        ret = false;
    }
    srcFile.close();
    desFile.close();
#ifdef Q_OS_UNIX
    SynExec("sync", "");
#endif
    return ret;
}

bool XSys::RmDir(const QString &dirpath)
{
    bool result = true;
    QDir dir(dirpath);

    if (dir.exists(dirpath)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = RmDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirpath);
    }
#ifdef Q_OS_UNIX
    XSys::SynExec("sync", "");
#endif
    return result;
}

void XSys::MoveDir(const QString &oldName, const QString &newName)
{
    RmFile(newName);
    RmDir(newName);
    QDir dir(oldName);
    dir.rename(oldName, newName);
#ifdef Q_OS_UNIX
    XSys::SynExec("sync", "");
#endif
}

QString XSys::Resource(const QString& name) {
    QDir res = QDir(QApplication::applicationDirPath());
    res.cdUp();
    res.cd("Resources");
    return res.absoluteFilePath(name);
}
