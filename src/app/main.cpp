

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
#include <DWindow>
#include <dutility.h>

#include <bminterface.h>
#include <util/utils.h>

#include "bmwindow.h"

DUTIL_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

int main(int argc, char **argv)
{
#if defined(STATIC_LIB)
    DWIDGET_INIT_RESOURCE();
#endif

    DApplication app(argc, argv);
    app.setOrganizationName("deepin");
    app.setApplicationName("deepin-boot-maker");
    app.setApplicationDisplayName(DApplication::tr("Deepin Boot Maker"));
    app.setApplicationVersion("1.99.0");
    app.setTheme("light");

    const QString m_format = "%{time}{yyyyMMdd.HH:mm:ss.zzz}[%{type:1}][%{function:-40} %{line:-4} %{threadid:-8} ] %{message}\n";
    DLogManager::setLogFormat(m_format);
    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();
    qDebug() << "save log to:" << DLogManager::getlogFilePath();

    if (!app.setSingleInstance("deepinbootmaker")) {
        qDebug() << "another deppin music has started";
        exit(0);
    }

    app.setTheme("light");

#ifdef Q_OS_WIN
    Utils::loadFonts();
    app.setWindowIcon(QIcon(":/theme/light/image/deepin-boot-maker.ico"));
#endif
    Utils::loadTranslate();

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

