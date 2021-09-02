#include "resultTest.h"
#include "../libxsys/Common/Result.h"
#include "../libxsys/Cmd/Cmd.h"
#include <QStringList>


using namespace XSys;

void ResultTest::SetUp()
{

}

void ResultTest::TearDown()
{

}

TEST_F(ResultTest, result)
{
    Result result(0, "msg", "result", "cmd");
    EXPECT_TRUE(result.isSuccess());
    EXPECT_EQ(result.code(), 0);
    EXPECT_EQ(result.cmd(), "cmd");
    EXPECT_EQ(result.errmsg(), "msg");
    EXPECT_EQ(result.result(), "result");
}

TEST_F(ResultTest, cmd)
{
    Result result1 = SynExec("lsblk", "-f");
    EXPECT_TRUE(result1.isSuccess());
    Result result2;
    QStringList param;
    param << "-f";
    result2 = SynExec("lsblk", param);
    EXPECT_TRUE(result1.isSuccess());
    Result result3 = SynExec("aaaaaaaaaaaaaaaaaa", "sssss");
    EXPECT_FALSE(result3.isSuccess());
}
