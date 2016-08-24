/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QDebug>
#include <QDir>
#include <QLocalSocket>
#include <QStandardPaths>
#include <QLibraryInfo>
#include <QTranslator>
#include <QLocalServer>

#include <qpa/qplatformintegrationfactory_p.h>

#include "dapplication.h"
#include "dthememanager.h"
#include "private/dthemehelper.h"
#include "private/dapplication_p.h"

#ifdef Q_OS_UNIX
#include "startupnotificationmonitor.h"
#endif

#include <DPathBuf>

#define DXCB_PLUGIN_KEY "dxcb"

DUTIL_USE_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

DApplicationPrivate::DApplicationPrivate(DApplication *q) :
    DObjectPrivate(q)
{
#ifdef Q_OS_UNIX
    StartupNotificationMonitor *monitor = StartupNotificationMonitor::instance();
    QObject::connect(monitor, &StartupNotificationMonitor::appStartup, [this, q](const QString id) {
        m_monitoredStartupApps.append(id);
        q->setOverrideCursor(Qt::WaitCursor);
    });
    QObject::connect(monitor, &StartupNotificationMonitor::appStartupCompleted, [this, q](const QString id) {
        m_monitoredStartupApps.removeAll(id);
        if (m_monitoredStartupApps.isEmpty()) {
            q->setOverrideCursor(Qt::ArrowCursor);
        }
    });
#endif
    m_translator = new QTranslator(q);
    m_qtTranslator = new QTranslator(q);
}

DApplicationPrivate::~DApplicationPrivate()
{
    if (m_localServer) {
        m_localServer->close();
    }
}

QString DApplicationPrivate::theme() const
{
    return DThemeManager::instance()->theme();
}

void DApplicationPrivate::setTheme(const QString &theme)
{
    DThemeManager *themeManager = DThemeManager::instance();
    themeManager->setTheme(theme);
}

bool DApplicationPrivate::setSingleInstance(const QString &key)
{
    D_Q(DApplication);

    if (m_localServer) {
        return m_localServer->isListening();
    }

    QLocalSocket *localSocket = new QLocalSocket;
    localSocket->connectToServer(key);

    // if connect success, another instance is running.
    bool result = localSocket->waitForConnected(1000);
    localSocket->deleteLater();

    if (result) {
        return false;
    }

    // create local server
    m_localServer = new QLocalServer(q);
    m_localServer->removeServer(key);

    return m_localServer->listen(key);
}

DApplication::DApplication(int &argc, char **argv) :
    QApplication(argc, argv),
    DObject(*new DApplicationPrivate(this))
{
}

QString DApplication::theme() const
{
    D_DC(DApplication);

    return d->theme();
}

void DApplication::setTheme(const QString &theme)
{
    D_D(DApplication);

    d->setTheme(theme);
}

bool DApplication::setSingleInstance(const QString &key)
{
    D_D(DApplication);

    return d->setSingleInstance(key);
}

//! load translate file form system or application data path;
/*!
  \param localeFallback, a list of fallback locale you want load.
  \return load success
*/
bool DApplication::loadTranslator(QList<QLocale> localeFallback)
{
    D_D(DApplication);

    d->m_qtTranslator->load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    this->installTranslator(d->m_qtTranslator);

    QList<DPathBuf> translateDirs;
    auto appName = applicationName();

    //("/home/user/.local/share", "/usr/local/share", "/usr/share")
    auto dataDirs = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);
    for (const auto &path : dataDirs) {
        DPathBuf DPathBuf(path);
        translateDirs << DPathBuf / appName / "translations";
    }

    DPathBuf runDir(this->applicationDirPath());
    translateDirs << runDir.join("translations");

    DPathBuf currentDir(QDir::currentPath());
    translateDirs << currentDir.join("translations");

    for (auto &locale : localeFallback) {
        QString translateFilename = QString("%1_%2").arg(appName).arg(locale.name());
        for (auto &path : translateDirs) {
            QString translatePath = (path / translateFilename).toString();
            if (QFile::exists(translatePath + ".qm")) {
                qDebug() << "load translate" << translatePath;
                d->m_translator->load(translatePath);
                this->installTranslator(d->m_translator);
                return true;
            }
        }
    }
    qWarning() << "load translate failed" << "can not find qm files";
    return false;
}

bool DApplication::loadDXcbPlugin()
{
    Q_ASSERT_X(!qApp, "DApplication::loadDxcbPlugin", "Must call before QGuiApplication defined object");

    if (!QPlatformIntegrationFactory::keys().contains(DXCB_PLUGIN_KEY))
        return false;

    return qputenv("QT_QPA_PLATFORM", DXCB_PLUGIN_KEY);
}

bool DApplication::isDXcbPlatform()
{
    return qApp && qApp->platformName() == "dxcb";
}

DWIDGET_END_NAMESPACE
