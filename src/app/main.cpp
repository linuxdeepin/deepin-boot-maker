

#include <QDebug>
#include <QIcon>
#include <QProcess>

#ifdef Q_OS_WIN32
#include <QFontDatabase>
namespace Utils
{
void loadFonts();
}
#endif

#include <DLog>
#include <DApplication>
#include <dutility.h>

#include <bminterface.h>
#include <util/utils.h>

#include "bmwindow.h"

DUTIL_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

#ifdef Q_OS_MAC
static bool switchToRoot(QApplication &app) {
    QStringList allappargs = app.arguments();
    QProcess whoamip;
    whoamip.start("whoami");
    whoamip.waitForFinished();

    if(QString(whoamip.readAll()).remove("\r").remove("\n") != "root") {
        QString argsconc = "";
        QString argsconcSingleQuote = "";

        for(int i = 1; i < allappargs.size(); ++i) {
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
    Q_INIT_RESOURCE(blob);
#if defined(STATIC_LIB)
    DWIDGET_INIT_RESOURCE();
#endif

    DApplication::loadDXcbPlugin();

    DApplication app(argc, argv);
    app.setOrganizationName("deepin");
    app.setApplicationName("deepin-boot-maker");
    app.setApplicationVersion("2.0.0");
    app.setTheme("light");

#ifdef Q_OS_MAC
    if(switchToRoot(app))
        exit(0);
#endif

    const QString m_format = "%{time}{yyyyMMdd.HH:mm:ss.zzz}[%{type:1}][%{function:-40} %{line:-4} %{threadid:-8} ] %{message}\n";
    DLogManager::setLogFormat(m_format);
    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();
    qDebug() << "save log to:" << DLogManager::getlogFilePath();

    if (!app.setSingleInstance("deepinbootmaker")) {
        qDebug() << "another deppin boot maker has started";
        exit(0);
    }

#ifdef Q_OS_WIN
    Utils::loadFonts();
    app.setWindowIcon(QIcon(":/theme/light/image/deepin-boot-maker.ico"));
#endif
    app.loadTranslator();
    Utils::loadTranslate();
    app.setApplicationDisplayName(QObject::tr("Deepin Boot Maker"));

    qDebug() << "Deepin Boot Maker UI started.";

    BMWindow w;
    DUtility::moveToCenter(&w);
    w.show();

//    w.waitAuth();

    auto ret =  app.exec();
    BMInterface::instance()->stop();
    return ret;
}

#ifdef Q_OS_WIN32
namespace Utils
{
void loadFonts()
{
    QFontDatabase database;
    QStringList fontlist = database.families();

    QStringList preferList;
    preferList.append("Microsoft YaHei");
    preferList.append("微软雅黑");
    preferList.append("SimHei");
    preferList.append("黑体");

    foreach(QString font, preferList) {
        if (fontlist.contains(font)) {
            QFont newFont = QFont(font);
            qApp->setFont(newFont);
            return;
        }
    }
}
}
#endif

