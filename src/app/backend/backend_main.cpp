#include <QDebug>
#include <QCoreApplication>
#include <QCommandLineParser>

#include <DLog>

DUTIL_USE_NAMESPACE

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    QCoreApplication::setOrganizationName("deepin");
    QCoreApplication::setApplicationName("deepin-boot-maker-backend");
    QCoreApplication::setApplicationVersion("2.0");

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::tr("Deepin Boot Maker Backend help you install iso too disk and make it boot able"));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("imagefile", QCoreApplication::translate("main", "Source image file."));
    parser.addPositionalArgument("target", QCoreApplication::translate("main", "Target partition or disk."));

    // A boolean option with multiple names (-f, --force)
    QCommandLineOption forceOption(QStringList() << "F" << "format",
                                   QCoreApplication::translate("main", "Format target."));

    parser.addOption(forceOption);

    // An option with a value
    QCommandLineOption targetDirectoryOption(QStringList() << "t" << "target-directory",
            QCoreApplication::translate("main", "Copy all source files into <directory>."),
            QCoreApplication::translate("main", "directory"));
    parser.addOption(targetDirectoryOption);

    // Process the actual command line arguments given by the user
    parser.process(app);

    const QStringList args = parser.positionalArguments();

    if (args.length() < 2) {
        parser.showHelp();
//        return -2;
    }

    QString imagefile = args.at(0);
    QString traget = args.at(1);
    // source is args.at(0), destination is args.at(1)

//    bool showProgress = parser.isSet(showProgressOption);
//    bool force = parser.isSet(forceOption);
//    QString targetDir = parser.value(targetDirectoryOption);s

    qDebug() << imagefile << traget;
    return 0;
//    return app.exec();
}
