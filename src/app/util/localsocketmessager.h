#ifndef LOCALSOCKETMESSAGER_H
#define LOCALSOCKETMESSAGER_H

#include <QObject>

#include <QLocalSocket>

class QLocalServer;

class LocalSocketMessager : public QObject
{
    Q_OBJECT
public:
    explicit LocalSocketMessager(const QString &sendPath,
                                 const QString &revicePath,
                                 QObject *parent = 0);

signals:
    void newConnection();
    void sendError(QLocalSocket::LocalSocketError socketError);
    void processMessage(quint64 msgType, QByteArray data);

public slots:
    void initSendConnect();
    void sendMessage(quint64 t, const QByteArray &data);

protected slots:
    void handleNewConnection();

protected:
    QString m_sendPath;
    QString m_recivePath;

    QLocalSocket *socket = nullptr;
    QLocalServer *server = nullptr;
};

Q_DECLARE_METATYPE(QLocalSocket::LocalSocketError)

#endif // LOCALSOCKETMESSAGER_H
