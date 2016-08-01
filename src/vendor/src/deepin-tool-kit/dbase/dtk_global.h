#pragma once

#include <QtCore/qglobal.h>

#define DTK_NAMESPACE Dtk

#if !defined(DTK_NAMESPACE)
#   define DTK_BEGIN_NAMESPACE
#   define DTK_END_NAMESPACE
#   define DTK_USE_NAMESPACE
#else
#   define DTK_BEGIN_NAMESPACE namespace DTK_NAMESPACE {
#   define DTK_END_NAMESPACE }
#   define DTK_USE_NAMESPACE using namespace DTK_NAMESPACE;
#endif

#if defined(STATIC_LIB)
    #  define LIBDTKBASESHARED_EXPORT
#else
    #if defined(LIBDTKBASE_LIBRARY)
    #  define LIBDTKBASESHARED_EXPORT Q_DECL_EXPORT
    #else
    #  define LIBDTKBASESHARED_EXPORT Q_DECL_IMPORT
    #endif
#endif

//! Dtk contians deepin tool kit
namespace Dtk {
}

