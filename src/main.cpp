/*
main.cpp from UNetbootin <http://unetbootin.sourceforge.net>
Copyright (C) 2007-2008 Geza Kovacs <geza0kovacs@gmail.com>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License at <http://www.gnu.org/licenses/> for more details.
*/
#include "usbcreator.h"
#include "unetbootin.h"
#include "qml/Deepin/Widgets/plugins/dwindow.h"
#include "qml/Deepin/Widgets/plugins/dicon.h"

#include <QQuickView>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlApplicationEngine>


#include <QtGui>
#include <QtWidgets/QApplication>

QString checkforgraphicalsu(QString graphicalsu)
{
	QProcess whereiscommand;
	whereiscommand.start(QString("which %1").arg(graphicalsu));
	whereiscommand.waitForFinished(-1);
	QString commandbinpath = QString(whereiscommand.readAll()).trimmed();
	if (!commandbinpath.isEmpty() && QFile::exists(commandbinpath))
		return commandbinpath;
	else
		return "REQCNOTFOUND";
}



#include <QtDebug>
#include <QFile>
#include <QTextStream>

void crashMessageOutput(QtMsgType type, const QMessageLogContext &, const QString & str)
{
    const char * msg = str.toStdString().c_str();
    QString txt;
    switch (type) {
        case QtDebugMsg:
            txt = QString("Debug: %1").arg(msg);
            break;
        case QtWarningMsg:
            txt = QString("Warning: %1").arg(msg);
            break;
        case QtCriticalMsg:
            txt = QString("Critical: %1").arg(msg);
            break;
        case QtFatalMsg:
            txt = QString("Fatal: %1").arg(msg);
            abort();
    }

    QFile outFile("deepin-usb-creator.log");
    outFile.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}

void loadTranslate(QApplication& app) {
    QTranslator *translator = new QTranslator();
    QString tnapplang;
    QString tnappcoun;
    QString clangcode = "";
    QStringList allappargs = app.arguments();
    QList<QPair<QString, QString> > oppairs;
    for (QList<QString>::const_iterator i = allappargs.constBegin(); i < allappargs.constEnd(); ++i) {
        if (i->count('=') == 1) {
            oppairs.append(QPair<QString, QString>(i->section('=', 0, 0).simplified(), i->section('=',1, 1).simplified()));
        }
    }
    for (QList<QPair<QString, QString> >::const_iterator i = oppairs.constBegin(); i < oppairs.constEnd(); ++i) {
        if (i->first.contains("lang", Qt::CaseInsensitive))
        {
            clangcode = i->second;
            tnapplang = clangcode.left(2);
            if (clangcode.contains('_') && clangcode.size() == 5)
            {
                tnappcoun = clangcode.section('_', -1, -1);
            }
            break;
        }
    }
    if (clangcode.isEmpty())
    {
        clangcode = QLocale::system().name();
        tnapplang = clangcode.left(2);
        if (clangcode.contains('_') && clangcode.size() == 5)
        {
            tnappcoun = clangcode.section('_', -1, -1);
        }
    }

    QString tranlateUrl = QString(":/deepin-usb-creator_%1_%2.qm").arg(tnapplang).arg(tnappcoun);
    if (!QFile::exists(tranlateUrl)) {
        tranlateUrl = ":/deepin-usb-creator_en_US.qm";
    }

    if (translator->load(tranlateUrl)){
        app.installTranslator(translator);
    }
}

#include <QtGlobal>
int main(int argc, char **argv)
{
    QApplication app(argc, argv, true);

    qInstallMessageHandler(crashMessageOutput);

    loadTranslate(app);

#ifdef Q_OS_UNIX
    bool disabledrootcheck = false;
    disabledrootcheck = true;
    QStringList allappargs = app.arguments();
    if (!disabledrootcheck)
    {
        QProcess whoamip;
        whoamip.start("whoami");
        whoamip.waitForFinished();
        if (QString(whoamip.readAll()).remove("\r").remove("\n") != "root")
        {
            QString argsconc = "";
            QString argsconcSingleQuote = "";
            for (int i = 1; i < allappargs.size(); ++i)
            {
                argsconc += QString("\"%1\" ").arg(allappargs.at(i));
                argsconcSingleQuote += QString("'%1' ").arg(allappargs.at(i));
            }
            argsconc += "\"rootcheck=no\"";
            argsconcSingleQuote += "'rootcheck=no'";
#ifdef Q_OS_LINUX
            QString gksulocation = checkforgraphicalsu("gksu");
            if (gksulocation != "REQCNOTFOUND")
            {
                QProcess::startDetached(QString("%1 %2 %3").arg(gksulocation).arg(app.applicationFilePath()).arg(argsconc));
                return 0;
            }
            QString kdesulocation = checkforgraphicalsu("kdesu");
            if (kdesulocation != "REQCNOTFOUND")
            {
                QProcess::startDetached(QString("%1 %2 %3").arg(kdesulocation).arg(app.applicationFilePath()).arg(argsconc));
                return 0;
            }
            QString gnomesulocation = checkforgraphicalsu("gnomesu");
            if (gnomesulocation != "REQCNOTFOUND")
            {
                QProcess::startDetached(QString("%1 %2 %3").arg(gnomesulocation).arg(app.applicationFilePath()).arg(argsconc));
                return 0;
            }
            QString kdesudolocation = checkforgraphicalsu("kdesudo");
            if (kdesudolocation != "REQCNOTFOUND")
            {
                QProcess::startDetached(QString("%1 %2 %3").arg(kdesudolocation).arg(app.applicationFilePath()).arg(argsconc));
                return 0;
            }
            QMessageBox rootmsgb;
            rootmsgb.setIcon(QMessageBox::Warning);
            rootmsgb.setWindowTitle(uninstaller::tr("Must run as root"));
            rootmsgb.setTextFormat(Qt::RichText);
            rootmsgb.setText(uninstaller::tr("%2 must be run as root. Close it, and re-run using either:<br/><b>sudo %1</b><br/>or:<br/><b>su - -c '%1'</b>").arg(app.applicationFilePath()).arg(UNETBOOTINB));
            rootmsgb.setStandardButtons(QMessageBox::Ok);
            switch (rootmsgb.exec())
            {
                case QMessageBox::Ok:
                    break;
                default:
                    break;
            }
#endif
#ifdef Q_OS_MAC
            /*
            QProcess osascriptProc;
            osascriptProc.start("osascript");
            osascriptProc.write(QString("do shell script \""+app.applicationFilePath()+"\" with administrator privileges\n").toAscii().data());
            osascriptProc.closeWriteChannel();
            osascriptProc.waitForFinished(-1);
            */
            //qDebug() << QString("osascript -e 'do shell script \"%1 %2\" with administrator privileges'").arg(app.applicationFilePath()).arg(argsconc);
            //QProcess::startDetached(QString("osascript -e 'do shell script \"%1 %2\" with administrator privileges'").arg(app.applicationFilePath()).arg(argsconc));
            QProcess::startDetached("osascript", QStringList() << "-e" << QString("do shell script \"'%1' %2\" with administrator privileges").arg(app.applicationFilePath()).arg(argsconcSingleQuote));
            return 0;
#endif
        }
    }
    #endif
    qmlRegisterType<UsbCreator>("com.deepin.usbcreator", 1, 0, "UsbCreator");
    qmlRegisterType<DOverrideWindow>("com.deepin.usbcreator", 1, 0, "DOverrideWindow");
    qmlRegisterType<DWindow>("com.deepin.usbcreator", 1, 0, "DWindow");
    qmlRegisterType<DIcon>("com.deepin.usbcreator", 1, 0, "DIcon");

    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/qml/");
    engine.load(QUrl("qrc:/qml/mainui.qml"));

    QList<QObject *> roots = engine.rootObjects();
    QObject *topLevel = roots.value(0);
    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);

    if (!window) {
        qCritical("load qrc:/qml/main.qml error!!");
    }

    QIcon icon;
    icon.addFile("qrc:/unetbootin_16.png", QSize(16,16));
    icon.addFile("qrc:/unetbootin_22.png", QSize(22,22));
    icon.addFile("qrc:/unetbootin_24.png", QSize(24,24));
    icon.addFile("qrc:/unetbootin_32.png", QSize(32,32));
    icon.addFile("qrc:/unetbootin_48.png", QSize(48,48));
    icon.addFile("qrc:/deepin-usb-creator.png");

    window->setIcon(icon);
    window->show();

    return app.exec();
}
