#ifndef QTLINUXINSTALLER_H
#define QTLINUXINSTALLER_H

#include "qtbaseinstaller.h"

#include <QObject>

class QtX86Installer : public QtBaseInstaller
{
    Q_OBJECT
public:
    explicit QtX86Installer(QObject *parent = nullptr);

protected:
    virtual bool installBootload() override;

signals:

public slots:
};

#endif // QTLINUXINSTALLER_H
