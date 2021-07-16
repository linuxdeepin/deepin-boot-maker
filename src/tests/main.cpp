#include <QCoreApplication>
#include <gtest/gtest.h>
#ifdef QT_DEBUG
#include <sanitizer/asan_interface.h>
#endif

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ::testing::InitGoogleTest(&argc,argv);
    int ret = RUN_ALL_TESTS();
#ifdef QT_DEBUG
    __sanitizer_set_report_path("asan_loader.log");
#endif
    return ret;
}
