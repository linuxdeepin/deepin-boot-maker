#include "dprogressframe.h"
#include "drotemovie.h"
#include "dusblist.h"
#include "dtips.h"
#include "dpushbutton.h"
#include "dserialframemovie.h"
#include "ddigitprogressmovie.h"
#include "dusbprogressmovie.h"

#include <QDebug>
#include <QLabel>
#include <QBoxLayout>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QStackedLayout>
#include <QFileDialog>
#include <QMovie>


QString TopShadowShow =
        "QWidget{"
            "background-color : qlineargradient(spread:reflect, x1:0, y1:0, x2:0, y2:1,"
            "stop:0 rgba(35, 38, 41, 255),"
            "stop:0.05 rgba(35, 38, 41, 0),"
            "stop:0.5 rgba(35, 38, 41, 0),"
            "stop:0.95 rgba(35, 38, 41, 0),"
            "stop:1 rgba(35, 38, 41, 255));"
        "}";


DProgressFrame::DProgressFrame(QWidget *parent) :
    QStackedWidget(parent)
{
    int m_height = 250;
    this->setFixedHeight(m_height);
    m_FirstWidget = new QWidget();

    m_FirstLayout = new QVBoxLayout();
    m_FirstLayout->setSpacing(0);
    m_FirstLayout->addSpacing(25 + 250 / 20);

    QStringList isoSourceList;
    for (int i = 1; i < 23; ++i) {
        isoSourceList.append(QString(":/ui/images/iso/%1.png").arg(i));
    }
    m_IsoLabel = new DSerialFrameMovie(isoSourceList, this);
    m_IsoLabel->start();
    m_IsoLabel->setDTips(tr("Select ISO"));
    m_IsoLabel->showDTips(true);
    m_FirstLayout->addWidget(m_IsoLabel);
    m_FirstLayout->setAlignment(m_IsoLabel, Qt::AlignCenter);
    connect(m_IsoLabel, SIGNAL(clicked()), this, SLOT(selectISO()));

    m_ProcessLabel =new QLabel();
    m_ProcessLabel->setPixmap(QPixmap(":/ui/images/process-inactive.png"));
    m_ProcessLabel->setFixedSize(64, 64);
    m_ProcessLabel->setAlignment(Qt::AlignCenter);
    m_FirstLayout->addWidget(m_ProcessLabel);
    m_FirstLayout->setAlignment(m_ProcessLabel, Qt::AlignCenter);

    m_ProcessUsb =new DUsbProgressMovie();

    QStringList usbSourceList;
    for (int i = 1; i < 23; ++i) {
        usbSourceList.append(QString(":/ui/images/usb/%1.png").arg(i));
    }
    m_UsbLabel = new DSerialFrameMovie(usbSourceList, this);
    m_UsbLabel->setDTips(tr("Select USB"));
    m_UsbLabel->showDTips(true);
    m_UsbLabel->start();
    m_FirstLayout->addWidget(m_UsbLabel);
    m_FirstLayout->setAlignment(m_UsbLabel, Qt::AlignCenter);
    connect(m_UsbLabel, SIGNAL(clicked()), this, SLOT(switchShowStatus()));
    m_FirstWidget->setLayout(m_FirstLayout);

    this->addWidget(m_FirstWidget);

    m_SecondWidget = new QWidget();
    m_SecondWidget->setFixedSize(220, 160);
    QVBoxLayout *secondLayout = new QVBoxLayout();
    m_UsbList =new DUsbList();
    secondLayout->addWidget(m_UsbList);
    secondLayout->setAlignment(m_UsbList, Qt::AlignCenter);
    connect(m_UsbList, SIGNAL(itemClick(QString)), this, SLOT(finishSelectDev(QString)));
    connect(m_UsbList, SIGNAL(selectDev(QString)), this, SLOT(usbDevSelected(QString)));

    m_SecondWidget->setLayout(secondLayout);

    this->addWidget(m_SecondWidget);

    m_TopShadow = new QWidget();
    m_TopShadow->setStyleSheet(TopShadowShow);
    m_TopShadow->setFixedHeight(m_height);
    this->addWidget(m_TopShadow);
    m_TopShadow->show();
    m_TopShadow->raise();
    m_TopShadow->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    m_Active = false;
    m_ShowStatus = ShowFirst;
    m_Speed = 500;
}

void DProgressFrame::finishSelectDev(const QString &) {
    slideProcess();
}

void DProgressFrame::usbDevSelected(const QString & dev) {
    if (dev.isEmpty()) {
        qDebug()<<"Start";
        m_UsbLabel->start();
    } else {
        qDebug()<<"Stop";
        m_UsbLabel->stop();
    }
    setUsbDev(dev);
}

void DProgressFrame::switchProgress() {
    QMovie *processMovie = new QMovie(":/ui/images/process-active.gif");
    m_ProcessLabel->setMovie(processMovie);
    processMovie->start();
    m_FirstLayout->addWidget(m_ProcessUsb);
    m_FirstLayout->setAlignment(m_ProcessUsb, Qt::AlignCenter);

    m_UsbLabel->hide();
    m_ProcessUsb->start();
}

void DProgressFrame::switchShowStatus() {
    switch (m_ShowStatus){
    case ShowFirst:
        this->slideUsbSeclect();
        break;
    case ShowSecond:
        this->slideProcess();
        break;
    default:
        break;
    }
}

void DProgressFrame::slideUsbSeclect() {
    if (m_Active) {
        return;
    }
    else {
        m_Active=true;
    }
    emit changedUsbSeclet();
    m_UsbLabel->showDTips(false);
    m_UsbLabel->ingonreLeaveEvent();

    int offsetx=frameRect().width(); //inherited from mother
    int offsety=frameRect().height();//inherited from mother
    m_SecondWidget->setGeometry(0, 0, offsetx, offsety);
    offsetx=0;
    offsety=offsety;

    //re-position the next widget outside/aside of the display area
    QPoint pnext=m_SecondWidget->pos();
    QPoint pnow=m_FirstWidget->pos();
    m_FirstWidget->move(pnow.x(), pnow.y()- offsety + 64);
    m_SecondWidget->move(pnext.x(), pnext.y() + 64);
    //make it visible/show
    m_SecondWidget->show();
    m_SecondWidget->raise();
    m_TopShadow->raise();
    //animate both, the now and next widget to the side, using movie framework
    QPropertyAnimation *animnow = new QPropertyAnimation(m_FirstWidget, "pos");
    animnow->setDuration(m_Speed);
    animnow->setEasingCurve(QEasingCurve::OutBack);
    animnow->setStartValue(QPoint(pnow.x(), pnow.y()));
    animnow->setEndValue(QPoint(offsetx+pnow.x(), -offsety+pnow.y() + 64));

    QPropertyAnimation *animnext = new QPropertyAnimation(m_SecondWidget, "pos");
    animnext->setDuration(m_Speed);
    animnext->setEasingCurve(QEasingCurve::OutBack);
    animnext->setStartValue(QPoint(pnext.x(), offsety+pnext.y()));
    animnext->setEndValue(QPoint(pnext.x(), pnext.y() + 64));

    m_AnimGroup = new QParallelAnimationGroup;
    m_AnimGroup->addAnimation(animnow);
    m_AnimGroup->addAnimation(animnext);

    connect(m_AnimGroup, SIGNAL(finished()),this,SLOT(slideUsbDone()));

    m_Active=true;
    m_AnimGroup->start();
}

void DProgressFrame::slideUsbDone() {
    m_ShowStatus = ShowSecond;
    m_Active = false;
}

void DProgressFrame::toggleShadow(){

}

void DProgressFrame::slideProcess() {
    if (m_Active) {
        return;
    }
    else {
        m_Active=true;
    }

    emit changedProgress();

    int offsetx=frameRect().width(); //inherited from mother
    int offsety=frameRect().height();//inherited from mother

    m_FirstWidget->setGeometry ( 0,  64 - offsety, offsetx, offsety );
    offsetx=0;
    offsety=offsety;

    //re-position the next widget outside/aside of the display area
    QPoint pnext=m_FirstWidget->pos();
    QPoint pnow=m_SecondWidget->pos();
   // m_firstWidget->move(pnext.x(), pnext.y()+ offsety - 64);
   // m_secondWidget->move(pnow.x(), pnow.y()+ offsety - 64);
    //make it visible/show
    m_FirstWidget->show();
    m_FirstWidget->raise();
    m_TopShadow->raise();
    //animate both, the now and next widget to the side, using movie framework
    QPropertyAnimation *animnext = new QPropertyAnimation(m_FirstWidget, "pos");
    animnext->setDuration(m_Speed);
    animnext->setEasingCurve(QEasingCurve::OutBack);
    animnext->setStartValue(QPoint(pnext.x(), pnext.y()));
    animnext->setEndValue(QPoint(pnext.x(), pnext.y() + offsety - 64));

    QPropertyAnimation *animnow = new QPropertyAnimation(m_SecondWidget, "pos");
    animnow->setDuration(m_Speed);
    animnow->setEasingCurve(QEasingCurve::OutBack);
    animnow->setStartValue(QPoint(pnow.x(), pnow.y()));
    animnow->setEndValue(QPoint(pnow.x(), pnow.y() + offsety - 64));

    m_AnimGroup = new QParallelAnimationGroup;
    m_AnimGroup->addAnimation(animnow);
    m_AnimGroup->addAnimation(animnext);

    connect(m_AnimGroup, SIGNAL(finished()),this,SLOT(slideProgressDone()));

    m_Active=true;
    m_AnimGroup->start();
}

void DProgressFrame::slideProgressDone(){
    m_UsbLabel->showDTips(true);
    m_ShowStatus = ShowFirst;
    m_Active = false;
}

void DProgressFrame::selectISO(){
    QFileDialog fileDlg(this);
    fileDlg.setViewMode(QFileDialog::Detail);
    fileDlg.setFileMode(QFileDialog::ExistingFile);
    fileDlg.setNameFilter("ISO (*.iso);;");
    if (QFileDialog::Accepted == fileDlg.exec()) {
        QString text = fileDlg.selectedFiles().first();
        setIsoFile(text);
        emit isoFileSelected(text);
        m_IsoLabel->stop();
        connect(m_UsbLabel, SIGNAL(clicked()), this, SLOT(switchShowStatus()));
    }
}

void DProgressFrame::refreshUsbDrivers(const QStringList& list) {
    m_UsbList->refreshDriverList(list);
}

QString DProgressFrame::usbDev() const
{
    return m_UsbDev;
}

void DProgressFrame::setUsbDev(const QString &usbDev)
{
    m_UsbDev = usbDev;
}

QString DProgressFrame::isoFile() const
{
    return m_IsoFile;
}

void DProgressFrame::setIsoFile(const QString &isoFile)
{
    m_IsoFile = isoFile;
}

void DProgressFrame::setProgress(int p) {
    m_ProcessUsb->setProgress(p);
}
