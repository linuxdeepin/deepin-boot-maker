#pragma once

#include <DWindow>
#include <QScopedPointer>

class BMWindowPrivate;
class BMWindow: public DTK_WIDGET_NAMESPACE::DWindow
{
    Q_OBJECT
public:
    BMWindow(QWidget *parent = nullptr);
    ~BMWindow();

private:
    QScopedPointer<BMWindowPrivate> d_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(d_ptr), BMWindow)
};
