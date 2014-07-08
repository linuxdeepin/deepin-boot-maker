#ifndef SYS_H
#define SYS_H

#include <QObject>

#ifdef Q_OS_WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif
#endif

class QFile;

class XSys : public QObject
{
    Q_OBJECT
public:
    explicit XSys(QObject *parent = 0);
    static QString RandString(const QString &str = "");
    static QString TmpFilePath(const QString &filename = "");
    static QString InsertTmpFile(const QByteArray &data = "");
    static QString SynExec(const QString &exec, const QString &param);
    static void RmFile(QFile &file);
    static void RmFile(const QString &filename);
    static void CpFile(const QString &srcName, const QString &desName);
signals:

public slots:

};

#endif // SYS_H
