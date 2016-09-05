#include "bmwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPropertyAnimation>
#include <QStackedLayout>

#include "util/bootmakeragent.h"
#include "view/setepindicatorbar.h"
#include "view/isoselectview.h"
#include "view/usbselectview.h"
#include "view/progressview.h"
#include "view/resultview.h"

const static QString debugQSS = "background-color: rgba(255, 0, 255, 20%);border: 1px solid;"
                                " border-radius: 3; border-color: rgba(0, 255, 0, 20%);";

class BMWindowData
{
public:
    BMWindowData(QWidget *parent)
    {
        q = parent;
    }

//    BootMakerAgent *bmAgent         = nullptr;

    QWidget *q                      = nullptr;
    QLabel *m_title                 = nullptr;
    QWidget *actionWidgets          = nullptr;
    ISOSelectView *isoWidget        = nullptr;
    QWidget *usbWidget              = nullptr;
    QWidget *progressWidget         = nullptr;
    QWidget *resultWidget           = nullptr;
};

static void slideWidget(QWidget *left, QWidget *right)
{
    right->show();
    left->show();
    int delay = 300;
    QRect leftStart = QRect(0, 0, left->width(), left->height());
    QRect leftEnd = leftStart;
    leftEnd.setX(-left->width());

    QPropertyAnimation *animation = new QPropertyAnimation(left, "geometry");
    animation->setDuration(delay);
    animation->setStartValue(leftStart);
    animation->setEndValue(leftEnd);
    animation->start();


    QRect rightStart = QRect(left->width(), 0, right->width(), right->height());
    QRect rightEnd = leftStart;
    leftEnd.setX(0);

    QPropertyAnimation *animation2 = new QPropertyAnimation(right, "geometry");
    animation2->setDuration(delay);
    animation2->setStartValue(rightStart);
    animation2->setEndValue(rightEnd);
    animation2->start();

    animation->connect(animation, &QPropertyAnimation::finished,
                       animation, &QPropertyAnimation::deleteLater);
    animation2->connect(animation2, &QPropertyAnimation::finished,
                        animation2, &QPropertyAnimation::deleteLater);

}

BMWindow::BMWindow(QWidget *parent): DWindow(parent)
{
    setTitlebarFixedHeight(40);

    d = new BMWindowData(this);
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    this->setContentLayout(mainLayout);

    auto *actionsLayout = new QStackedLayout;
    d->isoWidget = new ISOSelectView;
    d->isoWidget->setFixedSize(440, 476);

    d->usbWidget = new UsbSelectView;
    d->usbWidget->setFixedSize(440, 476);
    d->usbWidget->move(440, 0);

    d->progressWidget = new ProgressView;
    d->progressWidget->setFixedSize(440, 476);
    d->progressWidget->move(440, 0);

    d->resultWidget = new ResultView;
    d->resultWidget->setFixedSize(440, 476);
    d->resultWidget->move(440, 0);

    actionsLayout->addWidget(d->isoWidget);
    actionsLayout->addWidget(d->usbWidget);
    actionsLayout->addWidget(d->progressWidget);
    actionsLayout->addWidget(d->progressWidget);
    mainLayout->addLayout(actionsLayout);

    mainLayout->addSpacing(8);
    auto wsib = new StepIndicatorBar(4);
    mainLayout->addWidget(wsib);

    connect(d->isoWidget, &ISOSelectView::isoFileSelected, this, [ = ] {
        slideWidget(d->isoWidget, d->usbWidget);
        wsib->setActiveStep(1);
    });
}

BMWindow::~BMWindow()
{
}
