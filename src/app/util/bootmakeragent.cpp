#include "bootmakeragent.h"

#include <QDebug>
#include <QDataStream>
#include <QCoreApplication>

BootMakerAgent *BootMakerAgent::s_agent = nullptr;

static  QString s_localPathBk = "97edf4e4-8e04-438a-994b-1369f76827e6-backend-1";
static  QString s_localPathUI = "97edf4e4-8e04-438a-994b-1369f76827e6-ui-1";

class ProgressMsg
{
public:
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

BootMakerBackend::BootMakerBackend(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<QLocalSocket::LocalSocketError>();

    server = new QLocalServer(this);
    server->setSocketOptions(QLocalServer::WorldAccessOption);
    if (!server->listen(s_localPathBk)) {
        qDebug() << "Unable to start the server: " << server->errorString();
        qFatal("%s", server->errorString().toStdString().c_str());
    }
    connect(server, SIGNAL(newConnection()), this, SLOT(handleNewConnection()));

    socket = new QLocalSocket(this);
    connect(socket, static_cast<void(QLocalSocket::*)(QLocalSocket::LocalSocketError)>(&QLocalSocket::error),
    [ = ](QLocalSocket::LocalSocketError socketError) {
        /* TODO: exit backend */
        Q_UNUSED(socketError);
        socket->close();
        qCritical() << "Connect error" << socketError << socket->errorString();
        qApp->exit(111);
//        qFatal("%s", socket->errorString().toStdString().c_str());
    });

    socket->connectToServer(s_localPathUI);
}

void BootMakerBackend::handleNewConnection()
{
    qDebug() << "handleNewConnection";
}

void BootMakerBackend::reportSevenZipProgress(int current, int /*files*/, const QString &file)
{
    reportProgress(current * 4 / 5 + 10, 100, file, "");
}

void BootMakerBackend::reportProgress(int current, int total, const QString &title, const QString &description)
{
    qDebug() << "Report progresss" << current << total << title << description;
    ProgressMsg progressMsg(static_cast<quint32>(current),
                            static_cast<quint32>(total),
                            title, description);
    QByteArray data;
    QDataStream out(data);
    out.setVersion(QDataStream::Qt_5_4);
    out << progressMsg;
    sendMessage(ProgressUpdated, data);
}

void BootMakerBackend::sendRemovePartitionsChangedNotify(const QList<DeviceInfo> &list)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_4);
    out << list;
    sendMessage(RemovePartitionsChanged, data);
}

void BootMakerBackend::sendMessage(MessageType t, const QByteArray &data)
{
    if (!socket->isOpen()) {
        return;
    }

    qDebug() << "Send Message type:" << t << "size:" << data.size();

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_4);
    out << static_cast<quint64>(t);
    out << static_cast<quint64>(data.size());
    out << data;
    socket->write(block);
    socket->flush();
}

BootMakerAgent::BootMakerAgent(QObject *parent): QObject(parent)
{
    server = new QLocalServer();
    server->setSocketOptions(QLocalServer::WorldAccessOption);
    if (!server->listen(s_localPathUI)) {
        qDebug() << "Unable to start the server: " << server->errorString();
        return;
    }
    connect(server, &QLocalServer::newConnection,
            this, &BootMakerAgent::handleNewConnection);

    socket = new QLocalSocket();
    connect(socket, static_cast<void(QLocalSocket::*)(QLocalSocket::LocalSocketError)>(&QLocalSocket::error),
    [ = ](QLocalSocket::LocalSocketError socketError) {
        /* TODO: exit backend */
        Q_UNUSED(socketError);
        socket->close();
        qCritical() << "Connect error" << socketError << socket->errorString();
    });
}

void BootMakerAgent::handleNewConnection()
{
    qDebug() << "new connection in";
    QLocalSocket *clientConnection = server->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));
    initConnect();
    connect(clientConnection, &QLocalSocket::readyRead, this, [ = ]() {
        quint64 msgType = 0;
        quint64 blockSize = 0;
        QDataStream in(clientConnection);
        in.setVersion(QDataStream::Qt_5_4);
        QByteArray data;
        in >> msgType;
        in >> blockSize;
        in >> data;
        Q_ASSERT(data.size() == static_cast<int>(blockSize));
        processMessage(static_cast<MessageType>(msgType), data);
        /*Message msg = Message::formByteArray(data);
        qDebug() << msg.current << msg.total << msg.title << msg.description;*/
    });
}

void BootMakerAgent::processMessage(MessageType t, QByteArray data)
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
    default:
        qCritical() << "Unkonw message " << t << data;
        break;
    }
}

void BootMakerAgent::initConnect()
{
    if (!socket->isValid()) {
        socket->connectToServer(s_localPathBk);
    }
}
