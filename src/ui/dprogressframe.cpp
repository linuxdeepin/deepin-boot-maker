#include "dprogressframe.h"
#include "drotemovie.h"
#include "dusblist.h"
#include "dtips.h"
#include "dpushbutton.h"
#include "dserialframemovie.h"
#include "ddigitprogressmovie.h"
#include "dusbprogressmovie.h"

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
    m_firstWidget = new QWidget();

    firstLayout = new QVBoxLayout();
    firstLayout->setSpacing(0);
    firstLayout->addSpacing(25 + 250 / 20);

    QStringList isoSourceList;
    for (int i = 1; i < 23; ++i) {
        isoSourceList.append(QString(":/ui/images/iso/%1.png").arg(i));
    }
    m_isoLabel = new DSerialFrameMovie(isoSourceList, this);
    m_isoLabel->start();
    m_isoLabel->setDTips(tr("Select ISO"));
    m_isoLabel->showDTips(true);
    firstLayout->addWidget(m_isoLabel);
    firstLayout->setAlignment(m_isoLabel, Qt::AlignCenter);
    connect(m_isoLabel, SIGNAL(clicked()), this, SLOT(selectISO()));

    m_processLabel =new QLabel();
    m_processLabel->setPixmap(QPixmap(":/ui/images/process-inactive.png"));
    m_processLabel->setFixedSize(64, 64);
    m_processLabel->setAlignment(Qt::AlignCenter);
    firstLayout->addWidget(m_processLabel);
    firstLayout->setAlignment(m_processLabel, Qt::AlignCenter);

    m_processUsb =new DUsbProgressMovie();

    QStringList usbSourceList;
    for (int i = 1; i < 23; ++i) {
        usbSourceList.append(QString(":/ui/images/usb/%1.png").arg(i));
    }
    m_usbLabel = new DSerialFrameMovie(usbSourceList, this);
    m_usbLabel->setDTips(tr("Select USB"));
    m_usbLabel->showDTips(true);
    firstLayout->addWidget(m_usbLabel);
    firstLayout->setAlignment(m_usbLabel, Qt::AlignCenter);
    connect(m_usbLabel, SIGNAL(clicked()), this, SLOT(switchShowStatus()));
    m_firstWidget->setLayout(firstLayout);

    this->addWidget(m_firstWidget);

    m_secondWidget = new QWidget();
    m_secondWidget->setFixedSize(220, 160);
    QVBoxLayout *secondLayout = new QVBoxLayout();
    m_listView =new DUSBList();
    secondLayout->addWidget(m_listView);
    secondLayout->setAlignment(m_listView, Qt::AlignCenter);
    connect(m_listView, SIGNAL(itemClick(QString)), this, SLOT(finishSelectDev(QString)));
    connect(m_listView, SIGNAL(selectDev(QString)), this, SLOT(usbDevSelected(QString)));

    m_secondWidget->setLayout(secondLayout);

    this->addWidget(m_secondWidget);

    m_topShadow = new QWidget();
    m_topShadow->setStyleSheet(TopShadowShow);
    m_topShadow->setFixedHeight(m_height);
    this->addWidget(m_topShadow);
    m_topShadow->show();
    m_topShadow->raise();
    m_topShadow->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    m_Active = false;
    m_showStatus = ShowFirst;
    m_speed = 500;
}

void DProgressFrame::finishSelectDev(const QString &) {
    slideProcess();
}

void DProgressFrame::usbDevSelected(const QString & dev) {
    setUsbDev(dev);
}

void DProgressFrame::switchProgress() {
    QMovie *processMovie = new QMovie(":/ui/images/process-active.gif");
    m_processLabel->setMovie(processMovie);
    processMovie->start();
    firstLayout->addWidget(m_processUsb);
    firstLayout->setAlignment(m_processUsb, Qt::AlignCenter);

    m_usbLabel->hide();
    m_processUsb->start();
}

void DProgressFrame::switchShowStatus() {
    switch (m_showStatus){
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
    m_usbLabel->showDTips(false);
    m_usbLabel->ingonreLeaveEvent();

    int offsetx=frameRect().width(); //inherited from mother
    int offsety=frameRect().height();//inherited from mother
    m_secondWidget->setGeometry(0, 0, offsetx, offsety);
    offsetx=0;
    offsety=offsety;

    //re-position the next widget outside/aside of the display area
    QPoint pnext=m_secondWidget->pos();
    QPoint pnow=m_firstWidget->pos();
    m_firstWidget->move(pnow.x(), pnow.y()- offsety + 64);
    m_secondWidget->move(pnext.x(), pnext.y() + 64);
    //make it visible/show
    m_secondWidget->show();
    m_secondWidget->raise();
    m_topShadow->raise();
    //animate both, the now and next widget to the side, using movie framework
    QPropertyAnimation *animnow = new QPropertyAnimation(m_firstWidget, "pos");
    animnow->setDuration(m_speed);
    animnow->setEasingCurve(QEasingCurve::OutBack);
    animnow->setStartValue(QPoint(pnow.x(), pnow.y()));
    animnow->setEndValue(QPoint(offsetx+pnow.x(), -offsety+pnow.y() + 64));

    QPropertyAnimation *animnext = new QPropertyAnimation(m_secondWidget, "pos");
    animnext->setDuration(m_speed);
    animnext->setEasingCurve(QEasingCurve::OutBack);
    animnext->setStartValue(QPoint(pnext.x(), offsety+pnext.y()));
    animnext->setEndValue(QPoint(pnext.x(), pnext.y() + 64));

    m_animgroup = new QParallelAnimationGroup;
    m_animgroup->addAnimation(animnow);
    m_animgroup->addAnimation(animnext);

    connect(m_animgroup, SIGNAL(finished()),this,SLOT(slideUsbDone()));

    m_Active=true;
    m_animgroup->start();
}

void DProgressFrame::slideUsbDone() {
    m_showStatus = ShowSecond;
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

    m_firstWidget->setGeometry ( 0,  64 - offsety, offsetx, offsety );
    offsetx=0;
    offsety=offsety;

    //re-position the next widget outside/aside of the display area
    QPoint pnext=m_firstWidget->pos();
    QPoint pnow=m_secondWidget->pos();
   // m_firstWidget->move(pnext.x(), pnext.y()+ offsety - 64);
   // m_secondWidget->move(pnow.x(), pnow.y()+ offsety - 64);
    //make it visible/show
    m_firstWidget->show();
    m_firstWidget->raise();
    m_topShadow->raise();
    //animate both, the now and next widget to the side, using movie framework
    QPropertyAnimation *animnext = new QPropertyAnimation(m_firstWidget, "pos");
    animnext->setDuration(m_speed);
    animnext->setEasingCurve(QEasingCurve::OutBack);
    animnext->setStartValue(QPoint(pnext.x(), pnext.y()));
    animnext->setEndValue(QPoint(pnext.x(), pnext.y() + offsety - 64));

    QPropertyAnimation *animnow = new QPropertyAnimation(m_secondWidget, "pos");
    animnow->setDuration(m_speed);
    animnow->setEasingCurve(QEasingCurve::OutBack);
    animnow->setStartValue(QPoint(pnow.x(), pnow.y()));
    animnow->setEndValue(QPoint(pnow.x(), pnow.y() + offsety - 64));

    m_animgroup = new QParallelAnimationGroup;
    m_animgroup->addAnimation(animnow);
    m_animgroup->addAnimation(animnext);

    connect(m_animgroup, SIGNAL(finished()),this,SLOT(slideProgressDone()));

    m_Active=true;
    m_animgroup->start();
}

void DProgressFrame::slideProgressDone(){
    m_usbLabel->showDTips(true);
    m_showStatus = ShowFirst;
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
        //m_isoLabel->showDTips(false);
        m_isoLabel->stop();
        //m_usbLabel->showDTips(true);
        connect(m_usbLabel, SIGNAL(clicked()), this, SLOT(switchShowStatus()));
        m_usbLabel->start();
    }
}

void DProgressFrame::refreshUsbDrivers(const QStringList& list) {
    m_listView->refreshDriverList(list);
}
QString DProgressFrame::usbDev() const
{
    return m_usbDev;
}

void DProgressFrame::setUsbDev(const QString &usbDev)
{
    m_usbDev = usbDev;
}

QString DProgressFrame::isoFile() const
{
    return m_isoFile;
}

void DProgressFrame::setIsoFile(const QString &isoFile)
{
    m_isoFile = isoFile;
}

void DProgressFrame::setProgress(int p) {
    m_processUsb->setProgress(p);
}
