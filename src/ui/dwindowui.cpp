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

//static const int sLabelMaxWidth = 220;
static const int sButtonFixWidth = 100;
static const int sButtonFixHeight = 22;

DWindowUI *DWindowUI::s_MainWindow = NULL;
DWindowUI::DWindowUI(QWidget *parent) :
    QMainWindow(parent)
{
    s_MainWindow = this;
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_ShowWithoutActivating, true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
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

    initUI();

    connect(m_BootMaker, SIGNAL(listUsbDevice(QStringList)),
            this, SLOT(refrshUsbDriverList(QStringList)));
}

DWindowUI::~DWindowUI(){
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

void DWindowUI::initUI() {
    m_topLayout = new QVBoxLayout();

    QHBoxLayout *btlayout = new QHBoxLayout();
    m_closeButton = new DCloseButton();
    connect(m_closeButton, SIGNAL(clicked()), this, SLOT(close()));
    btlayout->setMargin(0);
    btlayout->addWidget(m_closeButton);
    btlayout->setAlignment(m_closeButton, Qt::AlignRight);

    QVBoxLayout *logolayout = new QVBoxLayout();
    QLabel *logolabel = new QLabel();
    logolabel->setPixmap(QPixmap(":/ui/images/logo.png"));
    logolayout->addWidget(logolabel);
    logolayout->setAlignment(logolabel, Qt::AlignCenter);
    logolayout->addSpacing(10);
    QLabel *versionlabel = new QLabel(QString("<a style='color:white; font-size:14px;font-weight:600;'>%1 </a> <a style='color:white; font-size:8px;'>v1.0</a>").arg(AppTitle()));
    logolayout->addWidget(versionlabel);
    logolayout->setAlignment(versionlabel, Qt::AlignCenter);
    logolayout->addStretch();

    m_topLayout->addLayout(btlayout);
    m_topLayout->addLayout(logolayout);

    m_progressLayout = new QVBoxLayout();
    m_progressFrame = new DProgressFrame();
    m_topLayout->addWidget(m_progressFrame);
    m_topLayout->setAlignment(m_progressFrame, Qt::AlignCenter);

    m_progressText = new QLabel();
    m_progressText->setText(QString(tr("<p style='color:white; font-size:12px;'>%1% has been completed</p>")).arg(0));
    m_progressLayout->addWidget(m_progressText);
    m_progressLayout->setAlignment(m_progressText, Qt::AlignCenter);
    m_progressText->hide();
    m_topLayout->addLayout(m_progressLayout);

    m_optionLayout = new QVBoxLayout();
    m_optionLayout->addSpacing(16);
    connect(this, SIGNAL(refrshUsbDrivers(QStringList)), m_progressFrame, SLOT(refreshUsbDrivers(QStringList)));
    connect(m_progressFrame, SIGNAL(changedProgress()), this, SLOT(enableStartButton()));
    connect(m_progressFrame, SIGNAL(changedUsbSeclet()), this, SLOT(disableStartButton()));
    connect(m_progressFrame, SIGNAL(selectEmptyUsb(bool)), this, SLOT(disableFormatOption(bool)));

    m_formatCheckBox = new DCheckBox(tr("<p style='color:#b4b4b4; font-size:12px;'>The making success rate can be improved by formatting USB drive.</p>"));
    m_formatCheckBox->setFixedWidth(200);
    m_formatCheckBox->setFixedHeight(54);
    m_formatCheckBox->setMaximumSize(240, 50);
    m_formatCheckBox->setDisabled(true);
    connect(m_formatCheckBox, SIGNAL(clicked()), this, SLOT(confirmFormat()));
    m_optionLayout->addWidget(m_formatCheckBox);
    m_optionLayout->setAlignment(m_formatCheckBox, Qt::AlignCenter);
    m_optionLayout->addSpacing(8);

    m_processHints = new QLabel("<p style='color:#b4b4b4; font-size:12px'>"
                                + tr("Please DO NOT remove the USB drive or shutdown while file is writing.")
                                + "<a>");
    m_processHints->setFixedWidth(230);
    m_processHints->setFixedHeight(45);
    m_processHints->setAlignment(Qt::AlignCenter);
    m_processHints->setWordWrap(true);
    m_optionLayout->addWidget(m_processHints);
    m_optionLayout->setAlignment(m_processHints, Qt::AlignCenter);
    m_processHints->hide();

    m_optionLayout->addSpacing(0);
    m_start = new DPushButton(tr("Start"));
    m_start->setFixedSize(sButtonFixWidth, sButtonFixHeight);
    connect(m_start, SIGNAL(clicked()), this, SLOT(start()));

    m_optionLayout->addWidget(m_start);
    m_optionLayout->setAlignment(m_start, Qt::AlignCenter);
    m_optionLayout->addSpacing(30);
    m_optionLayout->addStretch();

    m_topLayout->addLayout(m_optionLayout);
    QWidget *window = new QWidget(this);
    window->setLayout(m_topLayout);
    setCentralWidget(window);

    connect(this, SIGNAL(confirmFormatPrompt()),
            this, SLOT(popConfirmFormatPrompt()));

}

void DWindowUI::disableFormatOption(bool disbale) {
    m_formatCheckBox->setDisabled(!disbale);
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

    if (0 != m_BootMaker->start(isoPath, usbDev, format)) {
        return;
    }
    switchToProcessUI();
}

void DWindowUI::confirmFormat() {
    emit confirmFormatPrompt();
}

void DWindowUI::popConfirmFormatPrompt() {
    if (m_formatCheckBox->checked())
        m_formatCheckBox->setChecked(m_BootMaker->confirmFormatDlg());
    m_formatCheckBox->repaint();
}

void DWindowUI::refrshUsbDriverList(const QStringList &list) {
    qDebug()<<"lsit:"<<list;
    emit  refrshUsbDrivers(list);
}

void DWindowUI::checkProcess() {
    if (m_BootMaker->isFinish()) {
        m_progressText->setText(QString(tr("<p style='color:white; font-size:12px;'>%1% has been completed</p>")).arg(100));
        switchToEndUI();
        m_ProcessTimer->stop();
        m_progressFrame->setProgress(100);
    } else {
        m_progressFrame->setProgress(m_BootMaker->processRate());
        m_progressText->setText(QString(tr("<p style='color:white; font-size:12px;'>%1% has been completed</p>")).arg(m_BootMaker->processRate()));
    }
}

void DWindowUI::switchToProcessUI() {
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

void DWindowUI::switchToEndUI() {
    qDebug()<<"to End";

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
    QLabel *sucess = new QLabel();
    sucess->setPixmap(QPixmap(":/ui/images/success.png"));

    QLabel *congratulations = new QLabel(tr("<p style='color:#ffffff; font-weight:blod; font-size:18px'>Made successfully</p>"));
    QLabel *finishHints = new QLabel(tr("<p style='color:#b4b4b4; font-size:12px'>Please select USB drive to boot when installing Deepin OS.</p>"));
    finishHints->setFixedWidth(200);
    finishHints->setWordWrap(true);

    QLabel *tips = new QLabel(tr(
         "<a style='color:#b4b4b4; font-size:12px'>If failed, please try to disable EFI option.</a>"));
    tips->setFixedWidth(200);
    tips->setFixedHeight(30);
    tips->setWordWrap(true);
    m_topLayout->addSpacing(34);
    m_topLayout->addWidget(sucess);
    m_topLayout->addSpacing(16);
    m_topLayout->setAlignment(sucess, Qt::AlignCenter);
    m_topLayout->addWidget(congratulations);
    m_topLayout->setAlignment(congratulations, Qt::AlignCenter);
    m_topLayout->addSpacing(50);
    m_topLayout->addWidget(finishHints);
    m_topLayout->setAlignment(finishHints, Qt::AlignCenter);
    m_topLayout->addWidget(tips);
    m_topLayout->setAlignment(tips, Qt::AlignCenter);

    m_ActionLayout = new QHBoxLayout();
    DPushButton *rebootLater = new DPushButton(tr("Restart Later"));
    rebootLater->setFixedSize(sButtonFixWidth, sButtonFixHeight);
    DPushButton *rebootNow = new DPushButton(tr("Restart Now"));
    rebootNow->setTextColor("#ffffff");

    rebootNow->setFixedSize(sButtonFixWidth, sButtonFixHeight);
    connect(rebootLater, SIGNAL(clicked()), this, SLOT(close()));
    connect(rebootNow, SIGNAL(clicked()), m_BootMaker, SLOT(reboot()));

    m_ActionLayout->setMargin(0);
    m_ActionLayout->setSpacing(10);
    m_ActionLayout->addStretch();
    m_ActionLayout->addWidget(rebootLater);
    m_ActionLayout->addWidget(rebootNow);
    m_ActionLayout->addStretch();

    m_topLayout->addStretch();
    m_topLayout->addLayout(m_ActionLayout);
    m_topLayout->setAlignment(m_ActionLayout, Qt::AlignCenter);
    m_topLayout->addSpacing(40);
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
