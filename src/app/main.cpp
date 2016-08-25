#include <QDebug>

#define STATIC_LIB

#include <DLog>
#include <DApplication>
#include <DWindow>

#include "bmwindow.h"
#include "util/sevenzip.h"

DUTIL_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

int main(int argc, char **argv)
{
    DWIDGET_INIT_RESOURCE();

    DApplication app(argc, argv);

    const QString m_format = "%{time}{yyyyMMdd.HH:mm:ss.zzz}[%{type:1}][%{function:-35} %{line:-4} %{threadid} ] %{message}\n";
    DLogManager::setLogFormat(m_format);
    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();

    app.setTheme("light");

    qDebug() << "Deepin Boot Maker started.";

    BMWindow w;
    w.setFixedSize(440, 550);
    w.show();

    SevenZip sz;
    sz.start();

    return app.exec();
}
