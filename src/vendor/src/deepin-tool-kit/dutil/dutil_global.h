#pragma once

#include <dtk_global.h>

#define DUTIL_NAMESPACE Util
#define DTK_UTIL_NAMESPACE DTK_NAMESPACE::DUTIL_NAMESPACE

#if !defined(DUTIL_NAMESPACE)
#   define DUTIL_BEGIN_NAMESPACE
#   define DUTIL_END_NAMESPACE
#   define DUTIL_USE_NAMESPACE
#else
#   define DUTIL_BEGIN_NAMESPACE namespace DTK_NAMESPACE { namespace DUTIL_NAMESPACE {
#   define DUTIL_END_NAMESPACE }}
#   define DUTIL_USE_NAMESPACE using namespace DTK_UTIL_NAMESPACE;
#endif

DTK_BEGIN_NAMESPACE

//! Util contians base tool lib like log, config
namespace Util {

}

DTK_END_NAMESPACE

