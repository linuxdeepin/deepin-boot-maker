#include <QDebug>
#include <QProcess>

#include <QDesktopWidget>
#include <DLog>
#include <DApplication>
#include <DWindow>

#include "bmwindow.h"
#include "backend/bootmaker.h"
#include "util/sevenzip.h"
#include "util/bootmakeragent.h"
#include "util/utils.h"
#include "util/usbdevicemonitor.h"


DUTIL_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

static QRect PrimaryRect()
{
    QDesktopWidget *w = QApplication::desktop();
    return w->screenGeometry(w->primaryScreen());
}

static QString rootCommand(QCoreApplication &app)
{
    return QString("gksu \"%1  -d -n\"").arg(app.applicationFilePath());
}

static QString startBackend(QCoreApplication &app)
{
    QProcess *gksu = new QProcess();
    gksu->startDetached(rootCommand(app));
//    gksu->waitForStarted(-1);
    return "";
}


int main(int argc, char **argv)
{
    qRegisterMetaType<QList<DeviceInfo> >();

    DWIDGET_INIT_RESOURCE();

    DApplication app(argc, argv);
    app.setOrganizationName("deepin");
    app.setApplicationName("deepin-boot-maker");
    app.setApplicationDisplayName(DApplication::tr("Deepin Boot Maker"));
    app.setApplicationVersion("1.99.0");
    app.setTheme("light");

    QCommandLineParser parser;
    parser.setApplicationDescription("Deepin Boot Maker is a tool to help you make a startup usb disk.");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption optImageFile(QStringList() << "f" << "file",
                                    DApplication::tr("ISO image file"),
                                    "image-file");
    QCommandLineOption optKey(QStringList() << "k" << "key",
                              DApplication::tr("Communication key"),
                              "key");
    QCommandLineOption optDaemon(QStringList() << "d" << "daemon",
                                 DApplication::tr("Run in background"));
    QCommandLineOption optNoInteractive(QStringList() << "n" << "nointeractive",
                                        DApplication::tr("Do not run gui"));

    parser.addOption(optDaemon);
    parser.addOption(optNoInteractive);
    parser.addOption(optImageFile);
    parser.addOption(optKey);
    parser.addPositionalArgument("device", DApplication::tr("USB Device"));
    parser.process(app);

//    Utils::CommandDfParse();
    const QString m_format = "%{time}{yyyyMMdd.HH:mm:ss.zzz}[%{type:1}][%{function:-35} %{line:-4} %{threadid} ] %{message}\n";
    DLogManager::setLogFormat(m_format);
    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();

    app.setTheme("light");

    if (parser.isSet(optDaemon)) {
        qDebug() << parser.value(optDaemon)
                 << parser.value(optNoInteractive)
                 << parser.value(optImageFile)
                 << parser.value(optKey)
                 << parser.positionalArguments();

        BootMaker bm;

        qDebug() << "Deepin Boot Maker Backend Started";

        return app.exec();
    }

    qDebug() << "Deepin Boot Maker UI started.";
//    startBackend(app);
    BootMakerAgent::Init();

    BMWindow w;
    w.setFixedSize(440, 550);
    w.move(PrimaryRect().center() - w.geometry().center());
    w.show();

    return app.exec();
}
