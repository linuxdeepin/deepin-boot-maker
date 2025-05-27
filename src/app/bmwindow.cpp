// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "bmwindow.h"
#include "view/isoselectview.h"
#include "view/usbselectview.h"
#include "view/progressview.h"
#include "view/resultview.h"
#include "bminterface.h"
#include "backend/bmhandler.h"
#include <ddialog.h>
#include <DTitlebar>
#include <DPageIndicator>
#include <DApplication>
#include <DGuiApplicationHelper>
#include <DWindowManagerHelper>

#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QStackedLayout>
#include <QMessageBox>
#include <QApplication>
#include <QProcess>
#include <QThread>
#include <QKeyEvent>

const static QString debugQSS = "background-color: rgba(255, 0, 255, 20%);border: 1px solid;"
                                " border-radius: 3; border-color: rgba(0, 255, 0, 20%);";

class BMWindowPrivate
{
public:
    BMWindowPrivate(BMWindow *parent): q_ptr(parent) {}

    DLabel          *m_title        = nullptr;
    DWidget         *actionWidgets  = nullptr;
    ISOSelectView   *isoWidget      = nullptr;
    UsbSelectView   *usbWidget      = nullptr;
    ProgressView    *progressWidget = nullptr;
    ResultView      *resultWidget   = nullptr;
//    DDialog         *warnDlg        = nullptr;
    BMInterface     *interface      = nullptr;
    DPageIndicator  *wsib      = nullptr;

    BMWindow *q_ptr;
    Q_DECLARE_PUBLIC(BMWindow)
};

BMWindow::BMWindow(QWidget *parent)
    : BMWindowBaseClass(parent), d_ptr(new BMWindowPrivate(this))
{
    Q_D(BMWindow);
    qDebug() << "Initializing Boot Maker window";

    setFixedSize(440, 550);

    // TODO
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    d->interface = BMInterface::instance();
#else
    d->interface = &BMInterface::ref();
#endif
    qDebug() << "BMInterface initialized";

    // init about info
    QString descriptionText = tr("Boot Maker is a simple tool to write system image files into USB flash drives and other media.");
    QString acknowledgementLink = "https://www.deepin.org/acknowledgments/deepin-boot-maker#thanks";
    qApp->setProductName(tr("Boot Maker"));
    qApp->setApplicationAcknowledgementPage(acknowledgementLink);
//    qApp->setProductIcon(QIcon(":/theme/light/image/deepin-boot-maker.svg"));
    qApp->setProductIcon(QIcon::fromTheme("deepin-boot-maker"));
    qApp->setApplicationDescription(descriptionText);

    auto title = titlebar();
    this->setAccessibleName("BMWindow");
    title->setAccessibleName("BMWindow_titlebar");
    auto flags = windowFlags() & ~Qt::WindowMaximizeButtonHint;
    setWindowFlags(flags);
#ifdef Q_OS_WIN
    title->setMenuVisible(false);
#endif
    // TODO: read it from parent
#ifdef Q_OS_MAC
    titlebar()->setWindowFlags(flags);

    QPalette pal(palette());
    pal.setColor(QPalette::Window, QColor(255, 255, 255));

    setAutoFillBackground(true);
    setPalette(pal);
#endif

    setTitlebarShadowEnabled(false);
    title->setTitle("");
    title->setIcon(QIcon::fromTheme("deepin-boot-maker")/*QIcon(":/theme/light/image/deepin-boot-maker.svg")*/);
    title->setFixedHeight(50);

    auto viewWidth = 440;
    QStackedLayout *actionsLayout = new QStackedLayout;
    actionsLayout->setContentsMargins(0, 0, 0, 0);
    d->isoWidget = new ISOSelectView();
    d->isoWidget->setAccessibleName("centralWidget_isoSelectWidget");
    d->isoWidget->resize(440, 466);

    d->usbWidget = new UsbSelectView;
    d->usbWidget->setAccessibleName("centralWidget_usbSelectWidget");
    d->usbWidget->move(viewWidth, 0);
    d->usbWidget->resize(440, 466);

    d->progressWidget = new ProgressView;
    d->progressWidget->setAccessibleName("centralWidget_progressWidget");
    d->progressWidget->move(viewWidth, 0);
    d->progressWidget->resize(440, 466);

    d->resultWidget = new ResultView;
    d->resultWidget->setAccessibleName("centralWidget_resultWidget");
    d->resultWidget->move(viewWidth, 0);
    d->resultWidget->resize(440, 466);

    actionsLayout->addWidget(d->isoWidget);
    actionsLayout->addWidget(d->usbWidget);
    actionsLayout->addWidget(d->progressWidget);
    actionsLayout->addWidget(d->resultWidget);

    d->wsib = new DPageIndicator(this);
    d->wsib->setAccessibleName("DPageIndicator_wsib");
    d->wsib->setAutoFillBackground(true);
    d->wsib->setPageCount(4);
    d->wsib->setCurrentPage(0);
    d->wsib->setFixedHeight(34);
    d->wsib->setPointRadius(3);
    d->wsib->setSecondaryPointRadius(3);
    d->wsib->setPointDistance(12);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addLayout(actionsLayout);
    mainLayout->addWidget(d->wsib);
    auto centralWidget = new DWidget;
    centralWidget->setAccessibleName("BMWindow_centralWidget");
    centralWidget->resize(440, 466);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    m_pSAnimationGroup = new QSequentialAnimationGroup(this);
    slot_ThemeChange();

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::newProcessInstance, this, [ = ] {
        qDebug() << "New process instance detected, activating window";
        this->activateWindow();
    });
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            this, &BMWindow :: slot_ThemeChange);
    connect(d->isoWidget, &ISOSelectView::isoFileSelected, this, [ = ] {
        qDebug() << "ISO file selected, transitioning to USB selection view";
        slideWidget(d->isoWidget, d->usbWidget);
        setProperty("bmISOFilePath", d->isoWidget->isoFilePath());
        d->wsib->setCurrentPage(1);
    });

    connect(d->isoWidget,&ISOSelectView::isoFileSelected,d->usbWidget,&UsbSelectView::getIsoFileSelectedPath);

    connect(d->usbWidget, &UsbSelectView::deviceSelected, this, [ = ](const QString & partition, bool format) {
        qDebug() << "Device selected - Partition:" << partition << "Format:" << format;
#ifdef Q_OS_WIN
        setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint);
        closeflags = false;
        this->setVisible(true);
#endif

        DWindowManagerHelper::instance()->setMotifFunctions(windowHandle(), DWindowManagerHelper::FUNC_CLOSE, false);
        titlebar()->setQuitMenuDisabled(true);
        d->usbWidget->setEnabled(false);

#ifdef Q_OS_WIN
        slideWidget(d->usbWidget, d->progressWidget);
        d->wsib->setCurrentPage(2);
#endif

        auto isoFilePath = property("bmISOFilePath").toString();
        qDebug() << "Starting installation - ISO:" << isoFilePath << "Partition:" << partition << "Format:" << format;
        emit d->interface->startInstall(isoFilePath, "", partition, format);
        // Linux: wait for startInstallRet() polkit authorization check result, then animate the widget.
    });

#ifndef Q_OS_WIN
    // Check polkit authorization success, not availiable on windows.
    connect(d->interface, &BMInterface::startInstallRet, this, [ = ](bool success) {
        if (success) {
            qDebug() << "Installation authorization successful, transitioning to progress view";
            slideWidget(d->usbWidget, d->progressWidget);
            d->wsib->setCurrentPage(2);
        } else {
            qWarning() << "Installation authorization failed";

            d->usbWidget->resetStartInstall();
            d->usbWidget->setEnabled(true);
            titlebar()->setQuitMenuDisabled(false);
            DWindowManagerHelper::instance()->setMotifFunctions(windowHandle(), DWindowManagerHelper::FUNC_CLOSE, true);
        }
    });
#endif

    connect(d->progressWidget, &ProgressView::testCancel, this, [ = ] {
        qDebug() << "Installation cancelled by user";
        setWindowFlags(windowFlags() | Qt::WindowCloseButtonHint);
        d->resultWidget->updateResult(BMHandler::SyscExecFailed, "title", "description");
        slideWidget(d->progressWidget, d->resultWidget);
        d->wsib->setCurrentPage(2);
    });
    connect(d->usbWidget, &UsbSelectView::finish, this, [ = ](quint32 error, const QString & title, const QString & description) {
        qDebug() << "USB selection finished - Error:" << error << "Title:" << title;
        Qt::WindowFlags flags = Qt::WindowCloseButtonHint;
        flags |= Qt::WindowSystemMenuHint;
        flags |= Qt::WindowMinimizeButtonHint;
        titlebar()->setDisableFlags(flags);
        slideWidget(d->usbWidget, d->progressWidget);
        d->wsib->setCurrentPage(2);
        emit d->progressWidget->finish(0, error, title, description);
    });
    connect(d->usbWidget, &UsbSelectView::backToPrevUI, this, [=]{
        qDebug() << "User clicked back, returning to ISO selection";
        slideWidget(d->usbWidget, d->isoWidget, 1);
        d->wsib->setCurrentPage(0);
    });
    //diff mac，win，linux
    connect(d->progressWidget, &ProgressView::finish,
    this, [ = ](quint32 current, quint32 error, const QString & title, const QString & description) {
        qDebug() << "Progress finished - Current:" << current << "Error:" << error << "Title:" << title;
        if (error != BMHandler::NoError) {
            qWarning() << "Operation failed with error:" << error;
            titlebar()->setQuitMenuDisabled(false);
#ifdef Q_OS_WIN
            setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
            closeflags = true;
            this->setVisible(true);
#endif
#ifndef Q_OS_WIN
            DWindowManagerHelper::instance()->setMotifFunctions(windowHandle(), DWindowManagerHelper::FUNC_CLOSE, true);
#endif
            d->resultWidget->updateResult(error, title, description);
            slideWidget(d->progressWidget, d->resultWidget);
            d->wsib->setCurrentPage(3);
        }
#ifdef  Q_OS_LINUX
        if (error == BMHandler::NoError && current == 101) {
            qDebug() << "Operation completed successfully on Linux";
            titlebar()->setMenuVisible(true);
            titlebar()->setQuitMenuDisabled(false);
            DWindowManagerHelper::instance()->setMotifFunctions(windowHandle(), DWindowManagerHelper::FUNC_CLOSE, true);
            d->resultWidget->updateResult(error, title, description);
            slideWidget(d->progressWidget, d->resultWidget);
            d->wsib->setCurrentPage(3);
        }
#endif
#ifndef Q_OS_LINUX
#if defined Q_OS_Win
        qDebug() << "Operation completed on Windows";
        titlebar()->setQuitMenuDisabled(false);
        setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
        closeflags = true;
        this->setVisible(true);
        titlebar()->setMenuVisible(false);
#endif
        d->resultWidget->updateResult(error, title, description);
        slideWidget(d->progressWidget, d->resultWidget);
        d->wsib->setCurrentPage(3);

#endif
    });

    qDebug() << "Starting BMInterface";
    d->interface->start();
}

void BMWindow :: slot_ThemeChange()
{
    Q_D(BMWindow);
    qDebug() << "Theme change detected";
    DPalette pa;
    DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
    if (themeType == DGuiApplicationHelper::LightType) {
        qDebug() << "Applying light theme";
        pa = d->wsib->palette();
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        pa.setColor(DPalette::Background, QColor(255, 255, 255));
#else
        pa.setColor(DPalette::Window, QColor(255, 255, 255));
#endif
        d->wsib->setPalette(pa);
        d->wsib->setPointColor(QColor("#2CA7F8"));
//        d->wsib->setSecondaryPointColor(QColor("#96ACBD"));
        d->wsib->setSecondaryPointColor(QColor(150, 172, 189, 51));
    } else if (themeType == DGuiApplicationHelper::DarkType) {
        qDebug() << "Applying dark theme";
        pa = d->wsib->palette();
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        pa.setColor(DPalette::Background, QColor("#292929"));
#else
        pa.setColor(DPalette::Window, QColor("#292929"));
#endif
        d->wsib->setPalette(pa);
        d->wsib->setPointColor(QColor("#0082FA"));
//        d->wsib->setSecondaryPointColor(QColor("#555555"));
        d->wsib->setSecondaryPointColor(QColor(85, 85, 85, 102));
    }
}

BMWindow::~BMWindow()
{
    qDebug() << "Destroying Boot Maker window";
    m_pSAnimationGroup->deleteLater();
}

QString rootCommand()
{
    return QString("gksu \"%1  -d -n\"").arg(qApp->applicationFilePath());
}

void startBackend()
{
    qDebug() << "Starting backend process";
    QProcess::startDetached(rootCommand());
}

#if defined(Q_OS_WIN)
void BMWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "Close event received, closeflags:" << closeflags;
    if (closeflags == false) {
        event->ignore();
    } else {
        event->accept();
    }
}
#endif

void BMWindow::slideWidget(DWidget *left, DWidget *right, int iDirection)
{
    qDebug() << "Sliding widgets - Direction:" << iDirection;
    SlideAnimatoin* pSlideAnimation = new SlideAnimatoin;
    pSlideAnimation->initAnimation(left, right, iDirection);

    if (QAbstractAnimation::Stopped == m_pSAnimationGroup->state()) {
        for (int i = 0; i < m_pSAnimationGroup->animationCount(); i++) {
            m_pSAnimationGroup->animationAt(i)->deleteLater();
        }

        m_pSAnimationGroup->clear();
        m_pSAnimationGroup->addAnimation(pSlideAnimation);
        m_pSAnimationGroup->start();
    }
    else {
        m_pSAnimationGroup->addAnimation(pSlideAnimation);
    }
}

SlideAnimatoin::SlideAnimatoin(QObject* pParent):QParallelAnimationGroup (pParent)
{
    QObject::connect(this, &SlideAnimatoin::finished, this, &SlideAnimatoin::slot_AnimationGroupFinished);
}

SlideAnimatoin::~SlideAnimatoin()
{
}

void SlideAnimatoin::initAnimation(DWidget* pLeftWidget, DWidget* pRightWidget, int iDirection)
{
    qDebug() << "Initializing slide animation - Direction:" << iDirection;
    setLeftWidget(pLeftWidget);
    setRightWidget(pRightWidget);
    m_pLeftWidget->show();
    m_pRightWidget->show();
    int delay = 300;
    QRect leftStart = QRect(0, 0, m_pLeftWidget->width(), m_pLeftWidget->height());
    QRect leftEnd = QRect(iDirection*m_pLeftWidget->width(), 0, m_pLeftWidget->width(), m_pLeftWidget->height());

    QPropertyAnimation *animation = new QPropertyAnimation(m_pLeftWidget, "geometry");
    animation->setDuration(delay);
    animation->setStartValue(leftStart);
    animation->setEndValue(leftEnd);

    QRect rightStart = QRect(-iDirection*m_pLeftWidget->width(), 0, m_pRightWidget->width(), m_pRightWidget->height());
    QRect rightEnd = leftStart;
    leftEnd.setX(0);

    QPropertyAnimation *animation2 = new QPropertyAnimation(m_pRightWidget, "geometry");
    animation2->setDuration(delay);
    animation2->setStartValue(rightStart);
    animation2->setEndValue(rightEnd);
    addAnimation(animation);
    addAnimation(animation2);
}

void SlideAnimatoin::setLeftWidget(DWidget* pWidget)
{
    m_pLeftWidget = pWidget;
}

void SlideAnimatoin::setRightWidget(DWidget* pWidget)
{
    m_pRightWidget = pWidget;
}

void SlideAnimatoin::slot_AnimationGroupFinished()
{
    qDebug() << "Slide animation finished";
    for (int i = 0; i < this->animationCount(); i++) {
        this->animationAt(i)->deleteLater();
    }

    this->clear();

    if (m_pLeftWidget != nullptr) {
        m_pLeftWidget->hide();
    }

    if (m_pRightWidget != nullptr) {
        m_pRightWidget->show();
    }
}
