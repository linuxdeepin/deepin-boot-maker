#pragma once

#include <QDir>

#include "dutil_global.h"

DUTIL_BEGIN_NAMESPACE

class DPathBuf
{
public:
    DPathBuf(const QString &path)
    {
        m_path = path;
    }

    DPathBuf &operator/(const QString &p)
    {
        m_path += "/" + p;
        return *this;
    }

    DPathBuf &operator/(const char *p)
    {
        return operator /(QString(p));
    }

    DPathBuf &join(const QString &p)
    {
        return operator /(p);
    }

    QString toString() const
    {
        return QDir::toNativeSeparators(m_path);
    }

private:
    QString m_path;
};

DUTIL_END_NAMESPACE
