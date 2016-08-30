#include "setepindicatorbar.h"

#include <QHBoxLayout>
#include <QLabel>

StepIndicatorBar::StepIndicatorBar(int stepCount, QWidget *parent) : QFrame(parent)
{
    Q_ASSERT(stepCount > 0);

    setFixedHeight(26);
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(5);

    mainLayout->addStretch();
    for (int i = 0; i < stepCount; ++i) {
        QLabel *lb = new QLabel;
        lb->setPixmap(s_indicatorInactive);
        lb->resize(s_indicatorInactive.size());
        labels.push_back(lb);
        mainLayout->addWidget(lb, 0, Qt::AlignHCenter);
    }
    mainLayout->addStretch();

    currentSetp = 0;
    labels[currentSetp]->setPixmap(s_indicator);

}

void StepIndicatorBar::setActiveStep(int n)
{
    Q_ASSERT(n >= 0 && n < labels.size());
    labels[currentSetp]->setPixmap(s_indicatorInactive);
    currentSetp = n;
    labels[n]->setPixmap(s_indicator);
}
