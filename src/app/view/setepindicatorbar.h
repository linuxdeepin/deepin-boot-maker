#ifndef SETEPINDICATORBAR_H
#define SETEPINDICATORBAR_H

#include <QFrame>

class QLabel;

class StepIndicatorBar : public QFrame
{
    Q_OBJECT
public:
    explicit StepIndicatorBar(int stepCount, QWidget *parent = 0);

signals:

public slots:
    void setActiveStep(int n);

private:
    QVector<QLabel *> labels;
    int currentSetp             = -1;
    QPixmap s_indicator         = QPixmap(":/indicator_active.png");
    QPixmap s_indicatorInactive = QPixmap(":/indicator_inactive.png");
};

#endif // SETEPINDICATORBAR_H
