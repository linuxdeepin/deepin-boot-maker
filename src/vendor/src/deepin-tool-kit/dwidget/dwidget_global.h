/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#pragma once
#include <dtk_global.h>

#include <QtCore/QMetaMethod>

#define DWIDGET_NAMESPACE Widget
#define DTK_WIDGET_NAMESPACE DTK_NAMESPACE::Widget


#   define DWIDGET_BEGIN_NAMESPACE namespace DTK_NAMESPACE { namespace DWIDGET_NAMESPACE {
#   define DWIDGET_END_NAMESPACE }}
#   define DWIDGET_USE_NAMESPACE using namespace DTK_WIDGET_NAMESPACE;

namespace Dtk
{
namespace Widget
{

#if defined(STATIC_LIB)
#define DWIDGET_INIT_RESOURCE() \
    do { \
        Q_INIT_RESOURCE(icons); \
        Q_INIT_RESOURCE(dui_theme_dark); \
        Q_INIT_RESOURCE(dui_theme_light); } while (0)
#endif

}
}

#if defined(STATIC_LIB)
    #  define LIBDTKWIDGETSHARED_EXPORT
#else
    #if defined(LIBDTKWIDGET_LIBRARY)
    #  define LIBDTKWIDGETSHARED_EXPORT Q_DECL_EXPORT
    #else
    #  define LIBDTKWIDGETSHARED_EXPORT Q_DECL_IMPORT
    #endif
#endif

#define DTKWIDGET_DECL_DEPRECATED Q_DECL_DEPRECATED

#define D_THEME_INIT_WIDGET(className, ...) \
    DThemeManager * manager = DThemeManager::instance(); \
    this->setStyleSheet(this->styleSheet() + manager->getQssForWidget(#className)); \
    connect(manager, &DThemeManager::themeChanged, [=](QString) { \
        this->setStyleSheet(manager->getQssForWidget(#className)); \
    });\
    QStringList list = QString(#__VA_ARGS__).replace(" ", "").split(",");\
    const QMetaObject *self = metaObject();\
    foreach (const QString &str, list) {\
        if(str.isEmpty())\
            continue;\
        connect(this, self->property(self->indexOfProperty(str.toLatin1().data())).notifySignal(),\
        manager, manager->metaObject()->method(manager->metaObject()->indexOfMethod("updateQss()")));\
    }
