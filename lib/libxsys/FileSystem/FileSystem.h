#pragma once

#include <QString>
class QFile;

namespace XSys {
namespace FS {

QString TmpFilePath(const QString &filename = "");
QString InsertTmpFile(const QString &fileurl);
bool InsertFile(const QString &fileurl, const QString &fullpath);
bool InsertFileData(const QString &name, const QByteArray &data = "");
QString SynExec(const QString &exec, const QString &param, const QString &execPipeIn = "");
bool RmFile(QFile &file);
bool RmFile(const QString &filename);
bool CpFile(const QString &srcName, const QString &desName);
bool MoveDir(const QString &oldName, const QString &newName);
bool RmDir(const QString &dirpath);

}
}
