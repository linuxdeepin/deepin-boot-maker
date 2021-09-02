#ifndef QTBASEINSTALLERTEST_H
#define QTBASEINSTALLERTEST_H
#include <gtest/gtest.h>
#include <../libdbm/installer/qtbaseinstaller.h>

class QtBaseInstallerTest : public testing::Test
{
protected:
    // Sets up the test fixture.
    virtual void SetUp();

    // Tears down the test fixture.
    virtual void TearDown();
public:
    QtBaseInstaller *m_qtBaseInstaller;
};


#endif // QTBASEINSTALLERTEST_H
