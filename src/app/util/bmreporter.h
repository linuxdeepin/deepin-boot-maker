#ifndef BMREPORTER_H
#define BMREPORTER_H

#include <QObject>

#include <QLocalSocket>
#include <QLocalServer>

class BMReporter : public QObject
{
    Q_OBJECT
public:
    explicit BMReporter(QObject *parent = 0);

public slots:
    void reportSevenZipProgress(int current, int files, const QString &file);
    void reportProgress(int current, int total, const QString &title, const QString &description);

private:
    QLocalSocket *socket = nullptr;
};

class BMLinsterner : public QObject
{
    Q_OBJECT
public:
    explicit BMLinsterner(QObject *parent = 0);

public slots:
    void handleNewConnection();

signals:
    void notifyProgress(int current, int total, const QString &title, const QString &description);

private:
    QLocalServer *server = nullptr;
};


Q_DECLARE_METATYPE(QLocalSocket::LocalSocketError)

#endif // BMREPORTER_HH
