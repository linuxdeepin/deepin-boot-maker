#ifndef SYS_H
#define SYS_H

#include <QObject>

#ifdef Q_OS_WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif
#endif

class QFile;

class XSys : public QObject {
    Q_OBJECT
public:
    explicit XSys(QObject *parent = 0);
    static QString RandString(const QString &str = "");
    static QString TmpFilePath(const QString &filename = "");
    static QString InsertFileData(const QString &name, const QByteArray &data = "");
    static QString InsertTmpFile(const QString &fileurl);
    static QString InsertFile(const QString &fileurl, const QString &fullpath);
    static QString SynExec(const QString &exec, const QString &param, const QString &execPipeIn = "");
    static void RmFile(QFile &file);
    static void RmFile(const QString &filename);
    static bool CpFile(const QString &srcName, const QString &desName);
    static void MoveDir(const QString &oldName, const QString &newName);
    static bool RmDir(const QString &dirpath);

    static QString Resource(const QString& name);
signals:


public slots:

};

#endif // SYS_H
