#ifndef STARTUPNOTIFICATIONMONITOR_H
#define STARTUPNOTIFICATIONMONITOR_H

#include <QObject>
#include <QAbstractNativeEventFilter>

class StartupNotificationMonitor : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    static StartupNotificationMonitor* instance();

    ~StartupNotificationMonitor();
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);

signals:
    void appStartup(QString id);
    void appStartupCompleted(QString id);

protected:
    StartupNotificationMonitor();
};

#endif // STARTUPNOTIFICATIONMONITOR_H
