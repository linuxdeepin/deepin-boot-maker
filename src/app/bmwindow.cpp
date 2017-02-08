#include "bmwindow.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPropertyAnimation>
#include <QStackedLayout>
#include <QMessageBox>
#include <QApplication>
#include <QProcess>
#include <QLabel>

#include <ddialog.h>

#include "view/setepindicatorbar.h"
#include "view/isoselectview.h"
#include "view/usbselectview.h"
#include "view/progressview.h"
#include "view/resultview.h"

#include "bminterface.h"
#include "backend/bmhandler.h"

DWIDGET_USE_NAMESPACE

const static QString debugQSS = "background-color: rgba(255, 0, 255, 20%);border: 1px solid;"
                                " border-radius: 3; border-color: rgba(0, 255, 0, 20%);";

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
    animation2->connect(animation2, &QPropertyAnimation::finished,
                        left, &QWidget::hide);

}

class BMWindowPrivate
{
public:
    BMWindowPrivate(BMWindow *parent): q_ptr(parent) {}

    QLabel          *m_title        = nullptr;
    QWidget         *actionWidgets  = nullptr;
    ISOSelectView   *isoWidget      = nullptr;
    UsbSelectView   *usbWidget      = nullptr;
    ProgressView    *progressWidget = nullptr;
    ResultView      *resultWidget   = nullptr;
//    DDialog         *warnDlg        = nullptr;

    BMInterface     *interface      = nullptr;

    BMWindow *q_ptr;
    Q_DECLARE_PUBLIC(BMWindow)
};

BMWindow::BMWindow(QWidget *parent)
    : DWindow(parent), d_ptr(new BMWindowPrivate(this))
{
    Q_D(BMWindow);

    setFixedSize(440, 550);

    d->interface = BMInterface::instance();

    setWindowFlags(windowFlags() & ~ Qt::WindowSystemMenuHint);
//    setTitle(tr("Deepin Boot Maker"));
//    setTitleIcon(QPixmap(":/theme/light/image/deepin-boot-maker.svg"));
    auto icon = new QLabel();
    auto iconSize = 20;
    icon->setContentsMargins(5, 0, 0, 0);
    icon->setPixmap(QPixmap(":/theme/light/image/deepin-boot-maker.svg").scaled(iconSize, iconSize));
    setTitlebarWidget(icon, Qt::AlignLeft);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    setContentLayout(mainLayout);
    auto *actionsLayout = new QStackedLayout;
    mainLayout->addLayout(actionsLayout);
    d->isoWidget = new ISOSelectView();
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
    actionsLayout->addWidget(d->resultWidget);

    mainLayout->addSpacing(8);
    auto wsib = new StepIndicatorBar(3);
    mainLayout->addWidget(wsib);

    connect(d->isoWidget, &ISOSelectView::isoFileSelected, this, [ = ] {
        slideWidget(d->isoWidget, d->usbWidget);
        setProperty("bmISOFilePath", d->isoWidget->isoFilePath());
        wsib->setActiveStep(1);
    });

    connect(d->usbWidget, &UsbSelectView::deviceSelected, this, [ = ](const QString & partition, bool format) {
        slideWidget(d->usbWidget, d->progressWidget);
        wsib->setActiveStep(2);
        auto isoFilePath = property("bmISOFilePath").toString();
        qDebug() << isoFilePath
                 << partition
                 << format;
        d->interface->install(isoFilePath, "", partition, format);
    });

    connect(d->progressWidget, &ProgressView::testCancel, this, [ = ] {
//        USBFormatError,
//        USBSizeError,
//        USBMountFailed,
//        ExtractImgeFailed,
        d->resultWidget->updateResult(BMHandler::SyscExecFailed, "title", "description");
        slideWidget(d->progressWidget, d->resultWidget);
        wsib->setActiveStep(2);
    });

    connect(d->progressWidget, &ProgressView::finish,
    this, [ = ](quint32 error, const QString & title, const QString & description) {
        d->resultWidget->updateResult(error, title, description);
        slideWidget(d->progressWidget, d->resultWidget);
        wsib->setActiveStep(2);
    });

//    d->warnDlg = new Dtk::Widget::DDialog(this);
//    d->warnDlg->setWindowFlags(d->warnDlg->windowFlags() & ~Qt::WindowCloseButtonHint);
//    d->warnDlg->setFixedWidth(400);
//    d->warnDlg->setIcon(QMessageBox::standardIcon(QMessageBox::Warning));
//    d->warnDlg->setWindowTitle(tr("Format USB flash drive"));
//    d->warnDlg->setTextFormat(Qt::AutoText);
//    d->warnDlg->setMessage(tr("Please input root password."));
//    d->warnDlg->addButtons(QStringList() << tr("Retry") << tr("exit"));

//    d->isoWidget->hide();
//    emit d->isoWidget->isoFileSelected();
//    emit d->usbWidget->de/sviceSelected(",", false);
//    emit d->progressWidget->testCancel();
    d->interface->start();
}

BMWindow::~BMWindow()
{

}

QString rootCommand()
{
    return QString("gksu \"%1  -d -n\"").arg(qApp->applicationFilePath());
}

void startBackend()
{
    QProcess::startDetached(rootCommand());
}

