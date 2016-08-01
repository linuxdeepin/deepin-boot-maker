#include "dwidget.h"

#include "private/dwidget_p.h"

DWIDGET_BEGIN_NAMESPACE

DWidgetPrivate::DWidgetPrivate(DWidget *q) : DX11WidgetPrivate(q)
{

}

DWidget::DWidget(QWidget *parent): DWidget(*(new DWidgetPrivate(this)), parent)
{

}

DWidget::DWidget(DObjectPrivate &dd, QWidget *parent): DX11Widget(dd, parent)
{
}

DWIDGET_END_NAMESPACE
