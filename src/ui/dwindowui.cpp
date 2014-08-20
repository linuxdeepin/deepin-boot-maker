#include "dwindowui.h"

#include "dcombobox.h"
#include "dfilechooseinput.h"
#include "dcheckbox.h"
#include "dpushbutton.h"
#include "drotemovie.h"
#include "dprogressframe.h"
#include "dclosebutton.h"
#include "../bootmaker.h"

#include <QDebug>
#include <QPainter>
#include <QLayout>
#include <QLabel>
#include <QMovie>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QListView>
#include <QTimer>
#include <QMouseEvent>

const int labelMaxWidth = 220;
const int buttonFixWidth = 100;

DWindowUI::DWindowUI(QWidget *parent) :
    QMainWindow(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_ShowWithoutActivating, true);
    resize(310, 470);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window |Qt::WindowStaysOnTopHint);
    setFocusPolicy(Qt::NoFocus);
    bootMaker_ = new BootMaker(this);
    InitUI();
}

DWindowUI::~DWindowUI(){
    m_usbRefreshTimer->stop();
    processTimer_->stop();
}

void DWindowUI::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.save();
    p.drawPixmap(0,0, QPixmap(":/ui/images/uimini.png"));
    p.restore();
}

void DWindowUI::InitUI() {
    m_topLayout = new QVBoxLayout();

    QHBoxLayout *btlayout = new QHBoxLayout();
    m_closeButton = new DCloseButton();
    connect(m_closeButton, SIGNAL(clicked()), this, SLOT(close()));
    btlayout->setMargin(0);
    btlayout->addWidget(m_closeButton);
    btlayout->setAlignment(m_closeButton, Qt::AlignRight);

    QHBoxLayout *logolayout = new QHBoxLayout();
    QLabel *logolabel = new QLabel();
    logolabel->setPixmap(QPixmap(":/ui/images/logo.png"));
    logolayout->addSpacing(70);
    logolayout->addWidget(logolabel);
    QLabel *versionlabel = new QLabel("<p style='color:white; font-size:10px;'>0.99</p>");
    logolayout->addWidget(versionlabel);
    logolayout->setAlignment(versionlabel, Qt::AlignBottom);
    logolayout->addStretch();

    m_topLayout->addLayout(btlayout);
    m_topLayout->addLayout(logolayout);

    m_progressLayout = new QVBoxLayout();
    m_progressFrame = new DProgressFrame();
    m_topLayout->addWidget(m_progressFrame);
    m_topLayout->setAlignment(m_progressFrame, Qt::AlignCenter);

    m_progressText = new QLabel();
    m_progressText->setText(QString("<p style='color:white; font-size:12px;'>Has written %1% </p>").arg(bootMaker_->processRate()));
    m_progressLayout->addWidget(m_progressText);
    m_progressLayout->setAlignment(m_progressText, Qt::AlignCenter);
    m_progressText->hide();
    m_topLayout->addLayout(m_progressLayout);

    m_optionLayout = new QVBoxLayout();
    m_optionLayout->addSpacing(16);
    connect(this, SIGNAL(refrshUsbDrivers(QStringList)), m_progressFrame, SLOT(refreshUsbDrivers(QStringList)));
    connect(m_progressFrame, SIGNAL(changedProgress()), this, SLOT(enableStartButton()));
    connect(m_progressFrame, SIGNAL(changedUsbSeclet()), this, SLOT(disableStartButton()));

    m_formatCheckBox = new DCheckBox(tr("<p style='color:white; font-size:12px;'>Format USB flash disk before installation to improve the making success rate.</p>"));
    m_formatCheckBox->setFixedWidth(220);
    m_formatCheckBox->setFixedHeight(30);
    connect(m_formatCheckBox, SIGNAL(clicked()), this, SLOT(confirmFormat()));
    m_optionLayout->addWidget(m_formatCheckBox);
    m_optionLayout->setAlignment(m_formatCheckBox, Qt::AlignCenter);
    m_optionLayout->addSpacing(4);

    m_processHints = new QLabel(("<a style='color:#ffffff; font-size:12px'>Please </a>"
                                         "<a style='color:#ebab4c; font-size:12px'>DO NOT</a>"
                                         "<a style='color:#ffffff; font-size:12px'> remove the USB flash drive or shutdown while file is writing.<a>"));
    m_processHints->setFixedWidth(labelMaxWidth);
    m_processHints->setWordWrap(true);
    m_optionLayout->addWidget(m_processHints);
    m_optionLayout->setAlignment(m_processHints, Qt::AlignCenter);
    m_processHints->hide();

    m_optionLayout->addSpacing(4);
    m_start = new DPushButton(tr("Start"));
    m_start->setFixedWidth(buttonFixWidth);
    connect(m_start, SIGNAL(clicked()), this, SLOT(start()));

    m_optionLayout->addWidget(m_start);
    m_optionLayout->setAlignment(m_start, Qt::AlignCenter);
    m_optionLayout->addSpacing(10);
    m_optionLayout->addStretch();

    m_topLayout->addLayout(m_optionLayout);
    QWidget *window = new QWidget(this);
    window->setLayout(m_topLayout);
    setCentralWidget(window);

    m_usbRefreshTimer = new QTimer(this);
    m_usbRefreshTimer->setInterval(3000);
    connect(m_usbRefreshTimer, SIGNAL(timeout()), this, SLOT(refrshUsbDriverList()));
    m_usbRefreshTimer->start();
}

void DWindowUI::disableStartButton(){
    m_start->setEnabled(false);
}

void DWindowUI::enableStartButton(){
    m_start->setEnabled(true);
}

void DWindowUI::start() {
    QString isoPath = m_progressFrame->isoFile();
    QString usbDev = m_progressFrame->usbDev();
    bool format = m_formatCheckBox->checked();

    if (0 != bootMaker_->start(isoPath, usbDev, false, format)) {
        return;
    }
    SwitchToProcessUI();
}

void DWindowUI::confirmFormat() {
    if (m_formatCheckBox->checked())
        m_formatCheckBox->setChecked(bootMaker_->confirmFormatDlg());
}

void DWindowUI::fileSelect(const QString &filePath) {
    qDebug()<<filePath;
    isoLabel_->setPixmap(QPixmap(":/ui/images/iso-active.png"));
}

void DWindowUI::refrshUsbDriverList() {
    QStringList list = bootMaker_->listUsbDrives();
    emit  refrshUsbDrivers(list);
}

void DWindowUI::checkProcess() {
    if (bootMaker_->isFinish()) {
        m_progressText->setText(QString("<p style='color:white; font-size:12px;'>Has written %1% </p>").arg(100));
        SwitchToEndUI();
        processTimer_->stop();
        m_progressFrame->setProgress(100);
    } else {
        m_progressFrame->setProgress(bootMaker_->processRate());
        m_progressText->setText(QString("<p style='color:white; font-size:12px;'>Has written %1% </p>").arg(bootMaker_->processRate()));
    }
}

void DWindowUI::SwitchToProcessUI() {
    m_closeButton->setDisabled(true);
    m_formatCheckBox->hide();
    m_start->hide();

    m_progressText->show();
    m_processHints->show();

    processTimer_ = new QTimer(this);
    processTimer_->setInterval(5000);
    connect(processTimer_, SIGNAL(timeout()), this, SLOT(checkProcess()));
    processTimer_->start();

    m_progressFrame->switchProgress();
}

void DWindowUI::SwitchToEndUI() {

    m_closeButton->setDisabled(false);

    QLayoutItem *child;
    while ((child = m_optionLayout->takeAt(0)) != 0)  {
        if (child->widget()) {
            child->widget()->setVisible(false);
        }
    }

    m_progressText->hide();
    m_processHints->hide();
    m_progressFrame->hide();
    QLabel *congratulations = new QLabel(tr("<p style='color:#057aff; font-size:24px'>Congratulations!</p>"));
    QLabel *finishHints = new QLabel(tr("<p style='color:#ffffff; font-size:12px'>You have successfully created a boot disk and select your boot disk to install Deepin OS after restarted.</p>"));
    finishHints->setFixedWidth(labelMaxWidth);
    finishHints->setWordWrap(true);

    QLabel *tips = new QLabel(tr(
         "<a style='color:#057aff; font-size:12px'>Tips: </a>"
         "<a style='color:#ffffff; font-size:12px'>If t he boot failure, please close the BIOS EFO option</a>"));
    tips->setFixedWidth(labelMaxWidth);
    tips->setWordWrap(true);
    m_topLayout->addSpacing(60);
    m_topLayout->addWidget(congratulations);
    m_topLayout->setAlignment(congratulations, Qt::AlignCenter);
    m_topLayout->addSpacing(10);
    m_topLayout->addWidget(finishHints);
    m_topLayout->setAlignment(finishHints, Qt::AlignCenter);
    m_topLayout->addSpacing(20);
    m_topLayout->addWidget(tips);
    m_topLayout->setAlignment(tips, Qt::AlignCenter);

    actionLayout_ = new QHBoxLayout();
    DPushButton *rebootLater = new DPushButton(tr("Restart Later"));
    rebootLater->setFixedWidth(buttonFixWidth);
    DPushButton *rebootNow = new DPushButton(tr("Restart Now"));
    rebootNow->setFixedWidth(buttonFixWidth);
    connect(rebootLater, SIGNAL(clicked()), this, SLOT(close()));
    connect(rebootNow, SIGNAL(clicked()), bootMaker_, SLOT(reboot()));

    actionLayout_->setMargin(0);
    actionLayout_->setSpacing(20);
    actionLayout_->addStretch();
    actionLayout_->addWidget(rebootLater);
    actionLayout_->addWidget(rebootNow);
    actionLayout_->addStretch();

    m_topLayout->addStretch();
    m_topLayout->addLayout(actionLayout_);
    m_topLayout->setAlignment(actionLayout_, Qt::AlignCenter);
    m_topLayout->addSpacing(30);
}


void DWindowUI::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        pressed_ = true;
        pos_ = event->globalPos() - this->pos();
    }
}
void DWindowUI::mouseMoveEvent(QMouseEvent *event){
    if (pressed_) {
        move(event->globalPos() - pos_);
    }
}

void DWindowUI::mouseReleaseEvent(QMouseEvent *event)
{
    pressed_ = false;
    event->accept();
}
