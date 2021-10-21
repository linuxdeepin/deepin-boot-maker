/*
 * Copyright (C) 2017 ~ 2018 Wuhan Deepin Technology Co., Ltd.
 *
 * Author:     Iceyer <me@iceyer.net>
 *
 * Maintainer: Iceyer <me@iceyer.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
#include <DApplicationHelper>
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

    setFixedSize(440, 550);

    d->interface = BMInterface::instance();

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
    pal.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);
#endif

    setTitlebarShadowEnabled(false);
    title->setTitle("");
    title->setIcon(QIcon::fromTheme("deepin-boot-maker")/*QIcon(":/theme/light/image/deepin-boot-maker.svg")*/);
    title->setFixedHeight(50);

    auto viewWidth = 440;
    QStackedLayout *actionsLayout = new QStackedLayout;
    actionsLayout->setMargin(0);
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
    mainLayout->setMargin(0);
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
        this->activateWindow();
    });
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            this, &BMWindow :: slot_ThemeChange);
    connect(d->isoWidget, &ISOSelectView::isoFileSelected, this, [ = ] {
        slideWidget(d->isoWidget, d->usbWidget);
        setProperty("bmISOFilePath", d->isoWidget->isoFilePath());
        d->wsib->setCurrentPage(1);
    });

    connect(d->isoWidget,&ISOSelectView::isoFileSelected,d->usbWidget,&UsbSelectView::getIsoFileSelectedPath);

    connect(d->usbWidget, &UsbSelectView::deviceSelected, this, [ = ](const QString & partition, bool format) {
#ifdef Q_OS_WIN
        setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint);
        closeflags = false;
        this->setVisible(true);
#endif
        DWindowManagerHelper::instance()->setMotifFunctions(windowHandle(), DWindowManagerHelper::FUNC_CLOSE, false);
        titlebar()->setQuitMenuDisabled(true);
        slideWidget(d->usbWidget, d->progressWidget);
        d->wsib->setCurrentPage(2);
        auto isoFilePath = property("bmISOFilePath").toString();
        qDebug() << "call interface install" << partition << format;
        emit d->interface->startInstall(isoFilePath, "", partition, format);
    });
    connect(d->progressWidget, &ProgressView::testCancel, this, [ = ] {
        setWindowFlags(windowFlags() | Qt::WindowCloseButtonHint);
        d->resultWidget->updateResult(BMHandler::SyscExecFailed, "title", "description");
        slideWidget(d->progressWidget, d->resultWidget);
        d->wsib->setCurrentPage(2);
    });
    connect(d->usbWidget, &UsbSelectView::finish, this, [ = ](quint32 error, const QString & title, const QString & description) {
        Qt::WindowFlags flags = Qt::WindowCloseButtonHint;
        flags |= Qt::WindowSystemMenuHint;
        flags |= Qt::WindowMinimizeButtonHint;
        titlebar()->setDisableFlags(flags);
        slideWidget(d->usbWidget, d->progressWidget);
        d->wsib->setCurrentPage(2);
        emit d->progressWidget->finish(0, error, title, description);
    });
    connect(d->usbWidget, &UsbSelectView::backToPrevUI, this, [=]{
        slideWidget(d->usbWidget, d->isoWidget, 1);
        d->wsib->setCurrentPage(0);
    });
    //diff mac，win，linux
    connect(d->progressWidget, &ProgressView::finish,
    this, [ = ](quint32 current, quint32 error, const QString & title, const QString & description) {
        qDebug() << error << title << description << current;
        if (error != BMHandler::NoError) {
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

    d->interface->start();
}

void BMWindow :: slot_ThemeChange()
{
    Q_D(BMWindow);
    DPalette pa;
    DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
    if (themeType == DGuiApplicationHelper::LightType) {
        pa = d->wsib->palette();
        pa.setColor(DPalette::Background, QColor(255, 255, 255));
        d->wsib->setPalette(pa);
        d->wsib->setPointColor(QColor("#2CA7F8"));
//        d->wsib->setSecondaryPointColor(QColor("#96ACBD"));
        d->wsib->setSecondaryPointColor(QColor(150, 172, 189, 51));
    } else if (themeType == DGuiApplicationHelper::DarkType) {
        pa = d->wsib->palette();
        pa.setColor(DPalette::Background, QColor("#292929"));
        d->wsib->setPalette(pa);
        d->wsib->setPointColor(QColor("#0082FA"));
//        d->wsib->setSecondaryPointColor(QColor("#555555"));
        d->wsib->setSecondaryPointColor(QColor(85, 85, 85, 102));
    }
}

BMWindow::~BMWindow()
{
    m_pSAnimationGroup->deleteLater();
}

QString rootCommand()
{
    return QString("gksu \"%1  -d -n\"").arg(qApp->applicationFilePath());
}

void startBackend()
{
    QProcess::startDetached(rootCommand());
}

#if defined(Q_OS_WIN)
void BMWindow::closeEvent(QCloseEvent *event)
{
    if (closeflags == false) {
        event->ignore();
    } else {
        event->accept();
    }
}
#endif

void BMWindow::slideWidget(DWidget *left, DWidget *right, int iDirection)
{
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
