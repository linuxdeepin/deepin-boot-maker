#include "bootmakeragent.h"

#include <QDebug>
#include <QDataStream>
#include <QCoreApplication>

QString s_localPathBk = "97edf4e4-8e04-438a-994b-1369f76827e6-backend-1";
QString s_localPathUI = "97edf4e4-8e04-438a-994b-1369f76827e6-ui-1";

BootMakerAgent *BootMakerAgent::s_agent = nullptr;

class ProgressMsg
{
public:
    ProgressMsg() {}

    ProgressMsg(quint32 current, quint32 total, const QString &title, const QString &description)
        : current(current), total(total), title(title), description(description)
    {
    }

    quint32 current;
    quint32 total;
    QString title;
    QString description;
};

QDataStream &operator<<(QDataStream &out, const ProgressMsg &msg)
{
    out << msg.current << msg.total << msg.total << msg.description;
    return out;
}

QDataStream &operator>>(QDataStream &in, ProgressMsg &msg)
{
    quint32 current;
    quint32 total;
    QString title;
    QString description;
    in >> current >> total >> total >> description;
    msg = ProgressMsg(current, total, title, description);
    return in;
}

BootMakerBackendDaemon::BootMakerBackendDaemon(const QString &uiPath,
        const QString &daemonPath,
        QObject *parent)
    : LocalSocketMessager(uiPath, daemonPath, parent)
{
    connect(this, &BootMakerBackendDaemon::sendError,
    this, [ = ](QLocalSocket::LocalSocketError socketError) {
        /* TODO: exit backend */
        Q_UNUSED(socketError);
        qApp->exit(111);
    });


    connect(this, &BootMakerBackendDaemon::processMessage,
            this, &BootMakerBackendDaemon::handleProcessMessage);
}

void BootMakerBackendDaemon::reportSevenZipProgress(int current, int /*files*/, const QString &file)
{
    reportProgress(current * 4 / 5 + 10, 100, file, "");
}

void BootMakerBackendDaemon::reportProgress(int current, int total, const QString &title, const QString &description)
{
    qDebug() << "Report progresss" << current << total << title << description;
    ProgressMsg progressMsg(static_cast<quint32>(current),
                            static_cast<quint32>(total),
                            title, description);
    QByteArray data;
    QDataStream out(&data, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_4);
    out << progressMsg;
    sendMessage(ProgressUpdated, data);
}

void BootMakerBackendDaemon::sendRemovePartitionsChangedNotify(const QList<DeviceInfo> &list)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_4);
    out << list;
    sendMessage(RemovePartitionsChanged, data);
}

void BootMakerBackendDaemon::handleProcessMessage(quint64 t, QByteArray data)
{
    qDebug() << t << data;
    switch (t) {
    case StartMake: {
        QString image;
        QString device;
        QString partition;
        bool formatDevice;
        QDataStream in(data);
        in.setVersion(QDataStream::Qt_5_4);
        in >> image >> device >> partition >> formatDevice;
        qDebug() <<  image << device << partition << formatDevice;
        emit start(image, device, partition, formatDevice);
        break;
    }
    default:
        qCritical() << "Unkonw message " << t << data;
        break;
    }
}


BootMakerAgent::BootMakerAgent(const QString &uiPath,
                               const QString &daemonPath,
                               QObject *parent)
    : LocalSocketMessager(daemonPath, uiPath, parent)
{
    connect(this, &BootMakerAgent::processMessage,
            this, &BootMakerAgent::handleProcessMessage);
}

void BootMakerAgent::Init()
{
    if (!s_agent) {
        s_agent =  new BootMakerAgent(s_localPathUI, s_localPathBk);
    }
}

void BootMakerAgent::start(const QString &image, const QString &device, const QString &partition, bool formatDevice)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_4);
    out << image << device << partition << formatDevice;
    sendMessage(StartMake, data);
}

void BootMakerAgent::handleProcessMessage(quint64 t, QByteArray data)
{
    qDebug() << "Revice Message type:" << t << "size:" << data.size();
    switch (t) {
    case RemovePartitionsChanged: {
        QList<DeviceInfo> list;
        QDataStream in(data);
        in.setVersion(QDataStream::Qt_5_4);
        in >> list;
        emit notifyRemovePartitionsChanged(list);
        break;
    }
    case ProgressUpdated: {
        ProgressMsg progressMsg;
        QDataStream in(data);
        in.setVersion(QDataStream::Qt_5_4);
        in >> progressMsg;
        emit notifyProgress(progressMsg.current, progressMsg.total, progressMsg.title, progressMsg.description);
        break;
    }
    default:
        qCritical() << "Unkonw message " << t << data;
        break;
    }
}

