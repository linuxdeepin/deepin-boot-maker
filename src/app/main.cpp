#include <DLog>
#include <DApplication>
#include <DWindow>

DUTIL_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

int main(int argc, char **argv) {
    DApplication app(argc, argv);

    app.setTheme("light");

    DWindow w;
    w.setFixedSize(400, 400);
    w.show();

    return app.exec();
}
