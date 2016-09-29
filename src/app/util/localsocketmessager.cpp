#include "localsocketmessager.h"

#include <QLocalSocket>
#include <QLocalServer>

#include <QDataStream>

LocalSocketMessager::LocalSocketMessager(const QString &sendPath,
        const QString &revicePath,
        QObject *parent) : QObject(parent)
{
    qRegisterMetaType<QLocalSocket::LocalSocketError>();

    m_sendPath = sendPath;
    m_recivePath = revicePath;
    server = new QLocalServer(this);
    server->setSocketOptions(QLocalServer::WorldAccessOption);
    if (!server->listen(m_recivePath)) {
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
        emit sendError(socketError);
    });

    socket->connectToServer(m_sendPath);
}

void LocalSocketMessager::sendMessage(quint64 t, const QByteArray &data)
{
    qDebug() << "Send Message type:" << t << "size:" << data.size() << socket->isOpen();
    if (!socket->isOpen()) {
        return;
    }

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_4);
    out << static_cast<quint64>(t);
    out << static_cast<quint64>(data.size());
    out << data;
    socket->write(block);
    socket->flush();
}

void LocalSocketMessager::initSendConnect()
{
    if (!socket->isValid()) {
        socket->connectToServer(m_sendPath);
        qDebug() << socket->errorString();
    }
}

void LocalSocketMessager::handleNewConnection()
{
    emit newConnection();
    qDebug() << "new connection in";
    QLocalSocket *clientConnection = server->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));

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
        emit processMessage(msgType, data);
    });
}
