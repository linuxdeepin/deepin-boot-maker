#ifndef BMREPORTER_H
#define BMREPORTER_H

#include <QObject>

#include "localsocketmessager.h"
#include "usbdevicemonitor.h"

enum DaemonMessageType {
    RemovePartitionsChanged = 0,
    ProgressUpdated
};

enum UIMessageType {
    StartMake = 0
};

class BootMakerBackendDaemon : public LocalSocketMessager
{
    Q_OBJECT
public:

    explicit BootMakerBackendDaemon(const QString &uiPath,
                                    const QString &daemonPath,
                                    QObject *parent = 0);

signals:
    void start(const QString &image,
               const QString &device,
               const QString &partition,
               bool  formatDevice);

public slots:
    void reportSevenZipProgress(int current, int files, const QString &file);
    void reportProgress(int current, int total, const QString &title, const QString &description);
    void sendRemovePartitionsChangedNotify(const QList<DeviceInfo> &list);


    void handleProcessMessage(quint64 t, QByteArray data);

};

class BootMakerAgent : public LocalSocketMessager
{
    Q_OBJECT
public:
    explicit BootMakerAgent(const QString &uiPath,
                            const QString &daemonPath,
                            QObject *parent = 0);

    static void Init();

    static BootMakerAgent *Instance()
    {
        Q_ASSERT(s_agent != nullptr);
        return s_agent;
    }

public slots:
    void start(const QString &image,
               const QString &device,
               const QString &partition,
               bool  formatDevice);

signals:
    void notifyProgress(quint32 current, quint32 total, const QString &title, const QString &description);
    void notifyRemovePartitionsChanged(const QList<DeviceInfo> &list);

private:
    Q_DISABLE_COPY(BootMakerAgent)

    void handleProcessMessage(quint64 t, QByteArray data);
    static BootMakerAgent *s_agent ;
};

extern QString s_localPathBk;
extern QString s_localPathUI;


#endif // BMREPORTER_HH
