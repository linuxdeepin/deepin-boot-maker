#ifndef DISKAPITESTOBJ_H
#define DISKAPITESTOBJ_H

#include <gtest/gtest.h>
#include <backend/bootmaker.h>

class DiskApiTestObj : public testing::Test
{
protected:
    // Sets up the test fixture.
    virtual void SetUp();

    // Tears down the test fixture.
    virtual void TearDown();

public:
    BootMaker* m_pBootMaker;
};

#endif // DISKAPITESTOBJ_H
