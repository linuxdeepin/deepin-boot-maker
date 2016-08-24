#ifndef DUTILTESTER_H
#define DUTILTESTER_H

#include <QObject>

class TestDUtil: public QObject
{
    Q_OBJECT
public:
    TestDUtil();

private slots:
    void testLogPath();

    void testPathChange();
};

#endif // DUTILTESTER_H
