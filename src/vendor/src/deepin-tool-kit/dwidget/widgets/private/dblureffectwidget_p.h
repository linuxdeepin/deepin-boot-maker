#ifndef DBLUREFFECTWIDGET_P_H
#define DBLUREFFECTWIDGET_P_H

#include "private/dobject_p.h"
#include "dblureffectwidget.h"

DWIDGET_BEGIN_NAMESPACE

class DBlurEffectWidgetPrivate : public DObjectPrivate
{
public:
    DBlurEffectWidgetPrivate(DBlurEffectWidget *qq);

    int radius = 10;
    DBlurEffectWidget::BlurMode mode = DBlurEffectWidget::GaussianBlur;

private:
    D_DECLARE_PUBLIC(DBlurEffectWidget)
};

DWIDGET_END_NAMESPACE

#endif // DBLUREFFECTWIDGET_P_H
