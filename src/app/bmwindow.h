#ifndef BMWINDOW_H
#define BMWINDOW_H

#include <DWindow>

class BootMakerAgent;
class BMWindowData;

class BMWindow: public DTK_WIDGET_NAMESPACE::DWindow
{
    Q_OBJECT
public:
    BMWindow(QWidget *parent = nullptr);
    ~BMWindow();
private:
    BMWindowData *d     = nullptr;
    Q_DISABLE_COPY(BMWindow)
};

#endif // BMWINDOW_H
