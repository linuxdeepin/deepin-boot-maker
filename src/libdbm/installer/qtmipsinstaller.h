#ifndef QTMIPSINSTALLER_H
#define QTMIPSINSTALLER_H
#include "qtbaseinstaller.h"

class QtMipsInstaller : public QtBaseInstaller
{
public:
    QtMipsInstaller(QObject *parent = nullptr);

protected:
    virtual bool installBootload() override;
};

#endif // QTMIPSINSTALLER_H
