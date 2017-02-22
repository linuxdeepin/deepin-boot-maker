#ifndef SYSLINUX_H
#define SYSLINUX_H

#include <QObject>

class Syslinux : public QObject
{
    Q_OBJECT
public:
    explicit Syslinux(QObject *parent = 0);

signals:

public slots:
};

#endif // SYSLINUX_H