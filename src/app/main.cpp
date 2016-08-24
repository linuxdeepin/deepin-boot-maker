#include <QDebug>

#define STATIC_LIB

#include <DLog>
#include <DApplication>
#include <DWindow>

#include "bmwindow.h"

DUTIL_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

int main(int argc, char **argv)
{
    DApplication app(argc, argv);

    DWIDGET_INIT_RESOURCE();

    app.setTheme("light");

    BMWindow w;
    w.setFixedSize(440, 550);
    w.show();

//    QProcess sevenz;
//    sevenz.setProgram("7z");

//    QStringList args;
//    args << "x" << "-y"
//         << "/home/iceyer/Development/iso/atm-hengyin-amd64-2016-06-27.iso"
//         << "-o/media/iceyer/B5F1-ED27/"
//         << "-bsp3";
//    sevenz.setArguments(args);

//    sevenz.setStandardErrorFile("/tmp/test.fs");
//    sevenz.start();
//    sevenz.waitForStarted(-1);
//    sevenz.waitForFinished(-1);
//    qDebug() << sevenz.arguments().join(" ");

//    QFile tmp("/tmp/test.fs");
//    tmp.open(QIODevice::ReadWrite);

//    while (true) {
//        char data[64 * 1024];
//        qint64  readed = sevenz.read(data, 64 * 1024);
//        tmp.write(data, readed);
//        qDebug() << "sevenz.read" << readed;
//        if (readed == 0) {
//            break;
//        }
//    }
//    tmp.close();
//    return 0;

    return app.exec();
}
