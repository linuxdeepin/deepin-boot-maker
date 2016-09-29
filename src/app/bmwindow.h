#ifndef BMWINDOW_H
#define BMWINDOW_H

#include <DWindow>
#include <QScopedPointer>

class BootMakerAgent;
class BMWindowData;

class BMWindow: public DTK_WIDGET_NAMESPACE::DWindow
{
    Q_OBJECT
public:
    BMWindow(QWidget *parent = nullptr);
    ~BMWindow();
    void waitAuth();

private:
    QScopedPointer<BMWindowData> d;
    Q_DISABLE_COPY(BMWindow)
};

#endif // BMWINDOW_H
