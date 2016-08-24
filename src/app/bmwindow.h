#ifndef BMWINDOW_H
#define BMWINDOW_H

#include <DWindow>

class BMWindowData;
class BMWindow: public DTK_WIDGET_NAMESPACE::DWindow
{
    Q_OBJECT
public:
    BMWindow(QWidget* parent=nullptr);

private:
    BMWindowData    *d = nullptr;

    Q_DISABLE_COPY(BMWindow)
};

#endif // BMWINDOW_H
