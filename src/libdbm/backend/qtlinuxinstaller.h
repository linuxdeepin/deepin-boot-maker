#ifndef QTLINUXINSTALLER_H
#define QTLINUXINSTALLER_H

#include "qtbaseinstaller.h"

#include <QObject>

class QtLinuxInstaller : public QtBaseInstaller
{
    Q_OBJECT
public:
    explicit QtLinuxInstaller(QObject *parent = nullptr);

protected:
    virtual bool umountPartion() override;
    virtual bool umountDisk() override;
    virtual QString getMountPoint() override;
    virtual bool ejectDisk() override;

signals:

public slots:
};

#endif // QTLINUXINSTALLER_H
