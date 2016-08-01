#ifndef DWIDGET_H
#define DWIDGET_H

#include <QWidget>
#include <dobject.h>
#include <dwidget_global.h>


#include <dx11widget.h>

DWIDGET_BEGIN_NAMESPACE

class DWidgetPrivate;

class LIBDTKWIDGETSHARED_EXPORT DWidget : public DX11Widget
{
    Q_OBJECT
public:
    explicit DWidget(QWidget *parent = 0);
    explicit DWidget(DObjectPrivate &dd, QWidget *parent = 0);

private:
    D_DECLARE_PRIVATE(DWidget)
    Q_DISABLE_COPY(DWidget)
};

DWIDGET_END_NAMESPACE


#endif // DWIDGET_H
