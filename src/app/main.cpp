// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include <QDebug>
#include <QIcon>
#include <QProcess>

#include "bmwindow.h"
#include <bminterface.h>
#include <util/utils.h>
#include <QDebug>
#include <DLog>
#include <DApplication>
#include <DWidgetUtil>
#include <DGuiApplicationHelper>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <DApplicationSettings>
#endif


DCORE_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

#ifdef Q_OS_MAC
static bool switchToRoot(QApplication &app)
{
    QStringList allappargs = app.arguments();
    QProcess whoamip;
    whoamip.start("whoami");
    whoamip.waitForFinished();

    if (QString(whoamip.readAll()).remove("\r").remove("\n") != "root") {
        QString argsconc = "";
        QString argsconcSingleQuote = "";

        for (int i = 1; i < allappargs.size(); ++i) {
            argsconc += QString("\"%1\" ").arg(allappargs.at(i));
            argsconcSingleQuote += QString("'%1' ").arg(allappargs.at(i));
        }

        argsconc += "\"rootcheck=no\"";
        argsconcSingleQuote += "'rootcheck=no'";

        QProcess::startDetached("osascript", QStringList() << "-e" << QString("do shell script \"'%1' %2\" with administrator privileges").arg(app.applicationFilePath()).arg(argsconcSingleQuote));
        return true;

    }

    return false;
}

#endif

int main(int argc, char **argv)
{
    qInfo() << "Starting Boot Maker application";
    Utils::initResource();
#ifndef Q_OS_WIN
#if defined(DTK_STATIC_LIB)
    DWIDGET_INIT_RESOURCE();
#endif
#endif

    if (!QString(qgetenv("XDG_CURRENT_DESKTOP")).toLower().startsWith("deepin")) {
        qInfo() << "Setting XDG_CURRENT_DESKTOP to Deepin";
        setenv("XDG_CURRENT_DESKTOP", "Deepin", 1);
    }

//    DApplication::loadDXcbPlugin(); //去除为兼容wayland环境
    DApplication app(argc, argv);
    // app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    // app.setAttribute(Qt::AA_EnableHighDpiScaling);
    app.setOrganizationName("deepin");
    app.setApplicationName("deepin-boot-maker");
    app.setApplicationVersion(DApplication::buildVersion("20191031"));
//    app.setApplicationVersion(DApplication::buildVersion(VERSION));
//    app.setTheme("light");

#ifdef Q_OS_MAC
    qDebug() << "Checking root privileges on macOS";
    if (switchToRoot(app)) {
        qDebug() << "Switching to root user, exiting current instance";
        exit(0);
    }
#endif
    const QString m_format = "%{time}{yyyyMMdd.HH:mm:ss.zzz}[%{type:1}][%{function:-40} %{line:-4} %{threadid:-8} ] %{message}\n";
    DLogManager::setLogFormat(m_format);
    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();

#ifndef Q_OS_MAC
    qDebug() << "Setting up single instance check";
    qputenv("DTK_USE_SEMAPHORE_SINGLEINSTANCE", "1");
    if (!DGuiApplicationHelper::instance()->setSingleInstance(app.applicationName(), DGuiApplicationHelper::UserScope)) {
        qDebug() << "Another instance is already running, exiting";
        exit(0);
    }
#endif

#ifdef Q_OS_WIN
    qDebug() << "Loading fonts on Windows";
    Utils::loadFonts();
    app.setWindowIcon(QIcon(":/theme/light/image/deepin-boot-maker.svg"));
#endif
    qDebug() << "Loading translations";
    app.loadTranslator();
    Utils::loadTranslate();
    app.setApplicationDisplayName(QObject::tr("Boot Maker"));

    qDebug() << "Boot Maker UI starting";
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    DApplicationSettings savetheme;
#endif
    BMWindow w;
    Dtk::Widget::moveToCenter(&w);
    w.show();
//    w.waitAuth();
    auto ret =  app.exec();
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    qInfo() << "Stopping BMInterface instance";
    BMInterface::instance()->stop();
#else
    qInfo() << "Stopping BMInterface reference";
    BMInterface::ref().stop();
#endif
    qDebug() << "Application exiting with code:" << ret;
    return ret;
}

#ifdef Q_OS_WIN32
namespace Utils {
void loadFonts()
{
    qInfo() << "Loading preferred fonts on Windows";
    QFontDatabase database;
    QStringList fontlist = database.families();

    QStringList preferList;
    preferList.append("Microsoft YaHei");
    preferList.append("微软雅黑");
    preferList.append("SimHei");
    preferList.append("黑体");

    foreach (QString font, preferList) {
        if (fontlist.contains(font)) {
            qInfo() << "Setting application font to:" << font;
            QFont newFont = QFont(font);
            qApp->setFont(newFont);
            return;
        }
    }
    qWarning() << "No preferred fonts found, using system default";
}
}
#endif

