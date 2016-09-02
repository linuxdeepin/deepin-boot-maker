#include "bmwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "util/bmreporter.h"
#include "view/setepindicatorbar.h"
#include "view/isoselectview.h"
#include "view/usbselectview.h"

const static QString debugQSS = "background-color: rgba(255, 0, 255, 20%);border: 1px solid;"
                                " border-radius: 3; border-color: rgba(0, 255, 0, 20%);";

class BMWindowData
{
public:
    BMWindowData(QWidget *parent)
    {
        q = parent;
    }

    BMLinsterner *progressMonitor   = nullptr;

    QWidget *q                      = nullptr;
    QLabel *m_title                 = nullptr;
    QWidget *actionWidgets          = nullptr;
    QWidget *isoWidget              = nullptr;
};

BMWindow::BMWindow(QWidget *parent): DWindow(parent)
{
    setTitlebarFixedHeight(40);

    d = new BMWindowData(this);
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    this->setContentLayout(mainLayout);

    QHBoxLayout *actionsLayout = new QHBoxLayout;
//    d->isoWidget = new ISOSelectView;
    d->isoWidget = new UsbSelectView;
    d->isoWidget->setFixedHeight(476);
    actionsLayout->addWidget(d->isoWidget, 0, Qt::AlignCenter);

    mainLayout->addLayout(actionsLayout);

    mainLayout->addSpacing(8);
    auto wsib = new StepIndicatorBar(3);
    mainLayout->addWidget(wsib);

//    wsib->setStyleSheet(debugQSS);
//    d->isoWidget->setStyleSheet(debugQSS);







//    bmr = new BMLinsterner(this);
//    bmr->listen();
}

BMWindow::~BMWindow()
{
//    bmr->deleteLater();
}
