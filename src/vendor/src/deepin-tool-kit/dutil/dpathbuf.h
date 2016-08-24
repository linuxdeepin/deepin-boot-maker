#pragma once

#include <QDir>

#include "dutil_global.h"

DUTIL_BEGIN_NAMESPACE

class DPathBuf
{
public:
    DPathBuf(const QString &path)
    {
        m_path = QDir(path).absolutePath();
    }

    DPathBuf operator/(const QString &p) const
    {
        return DPathBuf(m_path + "/" + p);
    }

    DPathBuf &operator/=(const QString &p)
    {
        return join(p);
    }

    DPathBuf operator/(const char *p) const
    {
        return operator /(QString(p));
    }

    DPathBuf &operator/=(const char *p)
    {
        return operator /=(QString(p));
    }

    DPathBuf &join(const QString &p)
    {
        m_path += "/" + p;
        m_path = QDir(m_path).absolutePath();
        return *this;
    }

    QString toString() const
    {
        return QDir::toNativeSeparators(m_path);
    }

private:
    QString m_path;
};

DUTIL_END_NAMESPACE
