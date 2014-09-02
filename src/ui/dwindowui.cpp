#include "dwindowui.h"

#include "dcombobox.h"
#include "dfilechooseinput.h"
#include "dcheckbox.h"
#include "dpushbutton.h"
#include "drotemovie.h"
#include "dprogressframe.h"
#include "dclosebutton.h"
#include "../bootmaker.h"
#include "dui.h"

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
#include <QGraphicsDropShadowEffect>

// TODO: move all dui class to namespace DUI, and then remove this
// using namespace DUI;

const int labelMaxWidth = 220;
const int buttonFixWidth = 100;

DWindowUI::DWindowUI(QWidget *parent) :
    QMainWindow(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_ShowWithoutActivating, true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window |Qt::WindowStaysOnTopHint);
    setFocusPolicy(Qt::NoFocus);

    m_Margin = 10;
    m_Radius = 4;
    resize(310, 470);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0,0);
    shadow->setBlurRadius(m_Margin);
    shadow->setColor(DUI::ShadowColor);
    this->setGraphicsEffect(shadow);

    m_BootMaker = new BootMaker(this);
    InitUI();
}

DWindowUI::~DWindowUI(){
    m_usbRefreshTimer->stop();
    m_ProcessTimer->stop();
}

void DWindowUI::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect rect = this->rect().marginsRemoved(QMargins(m_Margin, m_Margin, m_Margin, m_Margin));
    QPoint topLeft(rect.x(), rect.y());
    QPoint bottomRight(rect.x() + rect.width(), rect.y() + rect.height());
    QPainterPath border;
    border.addRoundedRect(rect, m_Radius, m_Radius);

    QLinearGradient linearGradient(topLeft, QPoint(topLeft.x(), bottomRight.y()));
    linearGradient.setColorAt(0.0, DUI::BackgroundTopColor);
    linearGradient.setColorAt(0.2, DUI::BackgroundBottonColor);
    linearGradient.setColorAt(1.0, DUI::BackgroundBottonColor);

    QPen borderPen(DUI::BorderColor);
    painter.setBrush(QBrush(linearGradient));
    painter.strokePath(border, borderPen);
    painter.fillPath(border, QBrush(linearGradient));
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
    m_progressText->setText(QString("<p style='color:white; font-size:12px;'>Has written %1% </p>").arg(m_BootMaker->processRate()));
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

    if (0 != m_BootMaker->start(isoPath, usbDev, false, format)) {
        return;
    }
    SwitchToProcessUI();
}

void DWindowUI::confirmFormat() {
    if (m_formatCheckBox->checked())
        m_formatCheckBox->setChecked(m_BootMaker->confirmFormatDlg());
}

void DWindowUI::refrshUsbDriverList() {
    QStringList list = m_BootMaker->listUsbDrives();
    emit  refrshUsbDrivers(list);
}

void DWindowUI::checkProcess() {
    if (m_BootMaker->isFinish()) {
        m_progressText->setText(QString("<p style='color:white; font-size:12px;'>Has written %1% </p>").arg(100));
        SwitchToEndUI();
        m_ProcessTimer->stop();
        m_progressFrame->setProgress(100);
    } else {
        m_progressFrame->setProgress(m_BootMaker->processRate());
        m_progressText->setText(QString("<p style='color:white; font-size:12px;'>Has written %1% </p>").arg(m_BootMaker->processRate()));
    }
}

void DWindowUI::SwitchToProcessUI() {
    m_closeButton->setDisabled(true);
    m_formatCheckBox->hide();
    m_start->hide();

    m_progressText->show();
    m_processHints->show();

    m_ProcessTimer = new QTimer(this);
    m_ProcessTimer->setInterval(5000);
    connect(m_ProcessTimer, SIGNAL(timeout()), this, SLOT(checkProcess()));
    m_ProcessTimer->start();

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

    m_ActionLayout = new QHBoxLayout();
    DPushButton *rebootLater = new DPushButton(tr("Restart Later"));
    rebootLater->setFixedWidth(buttonFixWidth);
    DPushButton *rebootNow = new DPushButton(tr("Restart Now"));
    rebootNow->setFixedWidth(buttonFixWidth);
    connect(rebootLater, SIGNAL(clicked()), this, SLOT(close()));
    connect(rebootNow, SIGNAL(clicked()), m_BootMaker, SLOT(reboot()));

    m_ActionLayout->setMargin(0);
    m_ActionLayout->setSpacing(20);
    m_ActionLayout->addStretch();
    m_ActionLayout->addWidget(rebootLater);
    m_ActionLayout->addWidget(rebootNow);
    m_ActionLayout->addStretch();

    m_topLayout->addStretch();
    m_topLayout->addLayout(m_ActionLayout);
    m_topLayout->setAlignment(m_ActionLayout, Qt::AlignCenter);
    m_topLayout->addSpacing(30);
}


void DWindowUI::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        m_MousePressed = true;
        m_LastMousePos = event->globalPos() - this->pos();
    }
}
void DWindowUI::mouseMoveEvent(QMouseEvent *event){
    if (m_MousePressed) {
        move(event->globalPos() - m_LastMousePos);
    }
}

void DWindowUI::mouseReleaseEvent(QMouseEvent *event)
{
    m_MousePressed = false;
    event->accept();
}
