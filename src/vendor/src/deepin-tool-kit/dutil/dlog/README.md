# Dtk/Util/DLog

DLog is the log module of deepin tool kit for Qt/C++

Logger is a simple way to write the history of your application lifecycle to any target logging device (which is called Appender and may write to any target you will implement with it: console, text file, XML or something - you choose) and to map logging message to a class, function, source file and line of code which it is called from.

Some simple appenders (which may be considered an examples) are provided with the Logger itself: see ConsoleAppender and FileAppender documentation.

It supports using it in a multithreaded applications, so all of its functions are thread safe.

Logger internally uses the lazy-initialized singleton object and needs no definite initialization, but you may consider registering a log appender before calling any log recording functions or macros.

The library design of Logger allows you to simply mass-replace all occurrences of qDebug and similar calls with similar Logger macros (e.g. dDebug())

Note
    Logger uses a singleton global instance which lives through all the application life cycle and self-destroys destruction of the QCoreApplication (or QApplication) instance. It needs a QCoreApplication instance to be created before any of the Logger's functions are called.

## Usage

Just add pkgconfig in .pro file

````
unix {
    CONFIG+=link_pkgconfig
    PKGCONFIG+=dtkutil
}
````

### Example

````

#include <QCoreApplication>
#include <DLog>
using namespace Dtk::Log;
int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    // 1 you can use standrd deepin application log format
    // 1.1 log to console
    DLogManager::registerConsoleAppender();
    // 1.2 log to standrd deepin user cache path: ~/.cache/{organ}/{appname}/
    // app.setOrganizationName("dtk-test"); // must set
    // app.setApplicationName("dlog-example"); // must set
    // dInfo()<< "LogFile:" << DLogManager::getlogFilePath();
    // DLogManager::registerFileAppender();
    // 1.3 log to stdout and file
    // DLogManager::registerFileAppender();
    // DLogManager::registerConsoleAppender();
    // 2 Register your own logger format;
    //  ConsoleAppender* consoleAppender = new ConsoleAppender;
    //  consoleAppender->setFormat("[%{type:-7}] <%{Function}> %{message}\n");
    //  logger->registerAppender(consoleAppender);
    dInfo("Starting the application");
    int result = 1;
    dWarning() << "Something went wrong." << "Result code is" << result;
    return result;
}
````

\sa Dtk::Util::DLogManager


