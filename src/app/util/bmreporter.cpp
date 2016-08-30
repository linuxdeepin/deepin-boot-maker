#include "bmreporter.h"

#include <QDebug>
#include <QDataStream>

class Message
{
public:
    Message() {}
    Message(int current, int total, const QString &title, const QString &description)
        : current(current), total(total), title(title), description(description)
    {
    }

    int current;
    int total;
    QString title;
    QString description;

    QByteArray toByteArray() const
    {
        return QString("%1 %2 %3 %4").arg(current).arg(total)
               .arg(QString::fromLatin1(title.toLatin1().toBase64()))
               .arg(QString::fromLatin1(description.toLatin1().toBase64()))
               .toLatin1();
    }

    static Message formByteArray(const QByteArray &data)
    {
        Message msg;
        QStringList args = QString::fromLatin1(data).split(" ");
        if (args.length() != 4) {
            return msg;
        }
        msg.current = args.at(0).toInt();
        msg.total = args.at(1).toInt();
        msg.title = QString::fromLatin1(QByteArray::fromBase64(args.at(2).toLatin1()));
        msg.description = QString::fromLatin1(QByteArray::fromBase64(args.at(3).toLatin1()));
        return msg;
    }
};

BMReporter::BMReporter(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<QLocalSocket::LocalSocketError>();
    socket = new QLocalSocket(this);
    connect(socket, static_cast<void(QLocalSocket::*)(QLocalSocket::LocalSocketError)>(&QLocalSocket::error),
    [ = ](QLocalSocket::LocalSocketError socketError) {
        /* ... */
        qDebug() << "Connect error" << socketError << socket->errorString();
        socket->close();
    });

    socket->connectToServer("97edf4e4-8e04-438a-994b-1369f76827e6");
}

void BMReporter::reportSevenZipProgress(int current, int files, const QString &file)
{
    qDebug() << "report extract " << current << files << file;
    reportProgress(current * 4 / 5 + 10, 100, file, "");
}

void BMReporter::reportProgress(int current, int total, const QString &title, const QString &description)
{
    if (!socket->isOpen()) {
        return;
    }
    Message msg(current, total, title, description);
    QByteArray data = msg.toByteArray();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_4);
    out << static_cast<quint64>(data.size());
    out << data.constData();
    socket->write(block);
    socket->flush();
}

BMLinsterner::BMLinsterner(QObject *parent): QObject(parent)
{
    server = new QLocalServer(this);
    server->setSocketOptions(QLocalServer::WorldAccessOption);
    if (!server->listen("97edf4e4-8e04-438a-994b-1369f76827e6")) {
        qDebug() << "Unable to start the server: " << server->errorString();
        return;
    }
    qDebug() << server->errorString();
    connect(server, SIGNAL(newConnection()), this, SLOT(handleNewConnection()));
}

void BMLinsterner::handleNewConnection()
{
    qDebug() << "new connection in";
    QLocalSocket *clientConnection = server->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));

    connect(clientConnection, &QLocalSocket::readyRead, this, [ = ]() {
        quint64 blockSize = 0;
        QDataStream in(clientConnection);
        in.setVersion(QDataStream::Qt_5_4);
//        qDebug() << clientConnection->bytesAvailable();
        QByteArray data;
        in >> blockSize;
        in >> data;
//        qDebug() << blockSize << data.size() << clientConnection->bytesAvailable();
        Message msg = Message::formByteArray(data);
        qDebug() << msg.current << msg.total << msg.title << msg.description;
    });
}
