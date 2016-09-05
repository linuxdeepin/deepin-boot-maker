#ifndef DBLUREFFECTWIDGET_H
#define DBLUREFFECTWIDGET_H

#include "dwidget_global.h"
#include "dobject.h"

#include <QWidget>

DWIDGET_BEGIN_NAMESPACE

class DBlurEffectWidgetPrivate;

class DBlurEffectWidget : public QWidget, public DObject
{
    Q_OBJECT

    Q_PROPERTY(int radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(BlurMode mode READ mode WRITE setMode NOTIFY modeChanged)

public:
    /// TODO: To support MeanBlur, MedianBlur, BilateralFilter
    enum BlurMode {
        GaussianBlur
    };

    Q_ENUMS(BlurMode)

    explicit DBlurEffectWidget(QWidget *parent = 0);

    int radius() const;
    BlurMode mode() const;

public slots:
    void setRadius(int radius);
    void setMode(BlurMode mode);

signals:
    void radiusChanged(int radius);
    void modeChanged(BlurMode mode);

protected:
    DBlurEffectWidget(DBlurEffectWidgetPrivate &dd, QWidget *parent = 0);

    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    D_DECLARE_PRIVATE(DBlurEffectWidget)
};

DWIDGET_END_NAMESPACE

#endif // DBLUREFFECTWIDGET_H
