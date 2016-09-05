#ifndef BMREPORTER_H
#define BMREPORTER_H

#include <QObject>

#include <QLocalSocket>
#include <QLocalServer>

#include "usbdevicemonitor.h"

enum MessageType {
    RemovePartitionsChanged = 0,
    ProgressUpdated
};

class BootMakerBackend : public QObject
{
    Q_OBJECT
public:

    explicit BootMakerBackend(QObject *parent = 0);

public slots:
    void handleNewConnection();

    void reportSevenZipProgress(int current, int files, const QString &file);
    void reportProgress(int current, int total, const QString &title, const QString &description);
    void sendRemovePartitionsChangedNotify(const QList<DeviceInfo> &list);

private:
    void sendMessage(MessageType t, const QByteArray &data);

    QLocalSocket *socket = nullptr;
    QLocalServer *server = nullptr;
};

class BootMakerAgent : public QObject
{
    Q_OBJECT
public:
    explicit BootMakerAgent(QObject *parent = 0);

    static void Init()
    {
        if (!s_agent) {
            s_agent =  new BootMakerAgent;
        }
    }

    static BootMakerAgent *Instance()
    {
        Q_ASSERT(s_agent != nullptr);
        return s_agent;
    }

public slots:
    void handleNewConnection();

signals:
    void notifyProgress(int current, int total, const QString &title, const QString &description);
    void notifyRemovePartitionsChanged(const QList<DeviceInfo> &list);

private:
    Q_DISABLE_COPY(BootMakerAgent)

    void processMessage(MessageType t, QByteArray data);

    void initConnect();

    QLocalServer *server = nullptr;
    QLocalSocket *socket = nullptr;

    static BootMakerAgent *s_agent ;
};


Q_DECLARE_METATYPE(QLocalSocket::LocalSocketError)

#endif // BMREPORTER_HH
