#ifndef IMAGECHECKOBJ_H
#define IMAGECHECKOBJ_H

#include <gtest/gtest.h>
#include <backend/bootmaker.h>

class ImageCheckObj : public ::testing::Test
{
protected:
    // Sets up the test fixture.
    virtual void SetUp();

    // Tears down the test fixture.
    virtual void TearDown();

public:
    BootMaker* m_pBootMaker;
};

#endif // IMAGECHECKOBJ_H
