#include "dwindowui.h"

#include "dcombobox.h"
#include "dfilechooseinput.h"
#include "dcheckbox.h"
#include "dpushbutton.h"
#include "droteanimation.h"

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

const int labelMaxWidth =380;

DWindowUI::DWindowUI(QWidget *parent) :
    QMainWindow(parent)
{
    resize(680, 440);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    setAttribute(Qt::WA_TranslucentBackground, true);
    QString style = "DWindowUI {"
            "margin : 1px"
            "}";
    setStyleSheet(style);

    bootMaker_ = new BootMaker(this);
    InitUI();
}

DWindowUI::~DWindowUI(){
    usbTimer_->stop();
    processTimer_->stop();
}

void DWindowUI::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.save();
    p.drawPixmap(0,0, QPixmap(":/ui/images/uibackgound.png"));
    p.restore();
}

void DWindowUI::InitUI() {
    QHBoxLayout *top = new QHBoxLayout();
    AddProcessUI(top);
    top->setStretch(0, 230);
    AddOptionUI(top);
    top->setStretch(1, 430);
    QWidget *window = new QWidget(this);
    window->setLayout(top);
    setCentralWidget(window);

    usbTimer_ = new QTimer(this);
    usbTimer_->setInterval(3000);
    connect(usbTimer_, SIGNAL(timeout()), this, SLOT(refrshUsbDriverList()));
    usbTimer_->start();
}

void DWindowUI::AddProcessUI(QBoxLayout * top) {
    QVBoxLayout *process = new QVBoxLayout();

    QSpacerItem *sp = new QSpacerItem(64,64,QSizePolicy::Minimum, QSizePolicy::Expanding);
    process->addSpacerItem(sp);

    isoLabel_ = new DRoteAnimation(QPixmap(":/ui/images/iso-inactive.png"));
    process->addWidget(isoLabel_);
    process->setAlignment(isoLabel_, Qt::AlignCenter);

    processLabel_ =new QLabel();
    processLabel_->setPixmap(QPixmap(":/ui/images/process-inactive.png"));
    process->addWidget(processLabel_);
    process->setAlignment(processLabel_, Qt::AlignCenter);

    usbLabel_ =new QLabel("");
    QPixmap usbPixmap(":/ui/images/usb-inactive.png");
    usbLabel_->setFixedSize(usbPixmap.size());
    usbLabel_->setStyleSheet("QLabel { background : url(:/ui/images/usb-inactive.png); }");
    usbLabel_->setAlignment(Qt::AlignCenter);
    process->addWidget(usbLabel_);
    process->setAlignment(usbLabel_, Qt::AlignCenter);

    process->addSpacerItem(sp);

    top->addLayout(process);

}

void DWindowUI::AddOptionUI(QBoxLayout *top) {
    QVBoxLayout *option = new QVBoxLayout();
    option->setSpacing(7);

    //add close button
    QHBoxLayout *btlayout = new QHBoxLayout();
    QPushButton *closeBt = new QPushButton();
    QString style = "QPushButton{"
        "background:url(:/ui/images/window_close_normal.png);"
        "border:0px;"
    "}"
    "QPushButton:hover{"
        "background:url(:/ui/images/window_close_hover.png);"
        "border:0px;"
    "}"
    "QPushButton:pressed{"
        "background:url(:/ui/images/window_close_press.png);"
        "border:0px;"
    "}";

    closeBt->setStyleSheet(style);
    QPixmap pixmap (":/ui/images/window_close_normal.png");
    closeBt->setFixedSize(pixmap.size());

    connect(closeBt, SIGNAL(clicked()), this, SLOT(close()));

    btlayout->setMargin(3);
    btlayout->addWidget(closeBt);
    btlayout->setAlignment(closeBt, Qt::AlignRight);

    //Add Logo
    QHBoxLayout *logolayout = new QHBoxLayout();
    QLabel *logolabel = new QLabel();
    logolabel->setPixmap(QPixmap(":/ui/images/logo.png"));

    logolayout->addSpacing(50);
    logolayout->addWidget(logolabel);

    QLabel *versionlabel = new QLabel("<p style='color:white; font-size:10px;'>0.99</p>");
    logolayout->addWidget(versionlabel);
    logolayout->setAlignment(versionlabel, Qt::AlignBottom);
    logolayout->addStretch();

    option->addLayout(btlayout);
    option->addLayout(logolayout);

    //Add Hint
    QLabel *easylabel = new QLabel(tr("<p style='color:white; font-size:14px;'>Easy to use without redundancy</p>"));
    easylabel->setFixedWidth(labelMaxWidth);
    option->addWidget(easylabel);

    //Add Description
    QString descriptions = tr("<a style='color:#a7a7a7; font-size:11px;'>Welcome to Deepin Boot Maker. After setting a few options, you'll be able to create a Deepin OS Startup Disk, which supports both BIOS and </a>"
            "<a style='color:#ebab4c; font-size:11px;'>UEFI</a><a style='color:#a7a7a7; font-size:11px;'> boot.</a>");
    QLabel *descriptionslabel = new QLabel(descriptions);
    descriptionslabel->setFixedWidth(labelMaxWidth);
    descriptionslabel->setWordWrap(true);
    option->addWidget(descriptionslabel);

    //Add select option layout
    selectLayout_ = new QVBoxLayout();

    //Add select iso hint
    QLabel *selectISO = new QLabel(tr("<p style='color:white; font-size:12px;'>Select the ISO File:</p>"));
    selectISO->setFixedWidth(labelMaxWidth);
    selectLayout_->addWidget(selectISO);

    isoFile_ = new DFileChooseInput();
    isoFile_->setFixedHeight(22);
    isoFile_->setFixedWidth(210);
    connect(isoFile_, SIGNAL(filedSelected(QString)), this, SLOT(fileSelect(QString)));

    selectLayout_->addWidget(isoFile_);

    //Add select usb driver hint
    QLabel *selectUSB = new QLabel(tr("<p style='color:white; font-size:12px;'>Select the USB Flash Drive:</p>"));
    selectISO->setFixedWidth(labelMaxWidth);
    selectLayout_->addWidget(selectUSB);

    usbDriver_ = new DComboBox();
    usbDriver_->setFixedWidth(210);

    selectLayout_->addWidget(usbDriver_);


    formatDisk_ = new DCheckBox(tr("<p style='color:white; font-size:12px;'>Format USB flash disk before installation to improve the making success rate.</p>"));
    formatDisk_->setFixedWidth(labelMaxWidth);
    connect(formatDisk_, SIGNAL(clicked()), this, SLOT(confirmFormat()));

    selectLayout_->addWidget(formatDisk_);

    bisoMode_ = new DCheckBox(tr("<p style='color:white; font-size:12px;'>Support BIOS. Unselect here.</p>"));
    bisoMode_->setFixedWidth(labelMaxWidth);
    selectLayout_->addWidget(bisoMode_);

    option->addStretch();
    option->addLayout(selectLayout_);

    option->addSpacing(10);
    actionLayout_ = new QHBoxLayout();
    startBt_ = new DPushButton(tr("Start"));
    connect(startBt_, SIGNAL(clicked()), this, SLOT(start()));

    actionLayout_->addSpacing(50);
    actionLayout_->addWidget(startBt_);
    actionLayout_->addStretch();
    actionLayout_->setAlignment(startBt_, Qt::AlignCenter);

    option->addStretch();
    option->addLayout(actionLayout_);
    option->addStretch();
    option->addStretch();

    top->addLayout(option);
}

void DWindowUI::start() {
    QString isoPath = isoFile_->text();
    QString usbDev = usbDriver_->currentText();
    bool format = formatDisk_->checked();
    bool bios = bisoMode_->checked();

    if (0 == bootMaker_->start(isoPath, usbDev, bios, format)) {
        SwitchToProcessUI();
    }
}

void DWindowUI::confirmFormat() {
    if (formatDisk_->checked())
        formatDisk_->setChecked(bootMaker_->confirmFormatDlg());
}

void DWindowUI::fileSelect(const QString &filePath) {
    qDebug()<<filePath;
    isoLabel_->setPixmap(QPixmap(":/ui/images/iso-active.png"));
}

void DWindowUI::refrshUsbDriverList() {
    QString curText = usbDriver_->currentText();
    usbDriver_->clear();
    usbDriver_->addItem(curText);
    QStringList list = bootMaker_->listUsbDrives();
    usbDriver_->addItems(list);

    usbDriver_->removeItem(0);
    if (0 == list.size()) {
        usbLabel_->setStyleSheet("QLabel { background : url(:/ui/images/usb-inactive.png); }");
        usbDriver_->setCurrentText("");
    } else {
        usbLabel_->setStyleSheet("QLabel { background : url(:/ui/images/usb-active.png); }");
        if (list.contains(curText)) {
            usbDriver_->setCurrentText(curText);
        } else {
            usbDriver_->setCurrentText(list.first());
        }
    }
}

void DWindowUI::checkProcess() {
    if (bootMaker_->isFinish()) {
        usbLabel_->setText(QString("<p style='color:white; font-size:10px;'>100%  </p>").arg(bootMaker_->processRate()));
        SwitchToEndUI();
    } else {
        usbLabel_->setText(QString("<p style='color:white; font-size:10px;'>%1%  </p>").arg(bootMaker_->processRate()));
    }
}

void DWindowUI::SwitchToProcessUI() {
    QLayoutItem *child;
    while ((child = actionLayout_->takeAt(0)) != 0)  {
        if (child->widget()) {
            child->widget()->setVisible(false);
        }
    }
    while ((child = selectLayout_->takeAt(0)) != 0)  {
        if (child->widget()) {
            child->widget()->setVisible(false);
        }
    }

    QLabel *processHints = new QLabel(tr("<a style='color:#ffffff; font-size:12px'>Please </a>"
                                         "<a style='color:#ebab4c; font-size:12px'>DO NOT</a>"
                                         "<a style='color:#ffffff; font-size:12px'> remove the USB flash drive or shutdown while file is writing.<a>"));
    processHints->setFixedWidth(labelMaxWidth);
    processHints->setWordWrap(true);

    selectLayout_->addStretch();
    selectLayout_->addWidget(processHints);
    selectLayout_->setAlignment(processHints, Qt::AlignVCenter);

    usbLabel_->setText("<p style='color:white; font-size:10px;'>0%  </p>");
    QMovie *processMovie = new QMovie(":/ui/images/process-active.gif");
    processLabel_->setMovie(processMovie);
    processMovie->start();

    processTimer_ = new QTimer(this);
    processTimer_->setInterval(5000);
    connect(processTimer_, SIGNAL(timeout()), this, SLOT(checkProcess()));
    processTimer_->start();

    isoLabel_->start();
}

void DWindowUI::SwitchToEndUI() {
    QLayoutItem *child;
    while ((child = actionLayout_->takeAt(0)) != 0)  {
        if (child->widget()) {
            child->widget()->setVisible(false);
        }
    }
    while ((child = selectLayout_->takeAt(0)) != 0)  {
        if (child->widget()) {
            child->widget()->setVisible(false);
        }
    }

    QLabel *finishHints = new QLabel(tr(
         "<p style='color:#057aff; font-size:14px'>Congratulations!</p>"
         "<p style='color:#ffffff; font-size:12px'>You have successfully created a boot disk and select your boot disk to install Deepin OS after restarted.</p>"
         "<p style='color:#ffffff; font-size:12px'>Do you need to restart now?</p>"));
    finishHints->setFixedWidth(labelMaxWidth);
    finishHints->setWordWrap(true);

    selectLayout_->addStretch();
    selectLayout_->addWidget(finishHints);
    selectLayout_->setAlignment(finishHints, Qt::AlignVCenter);

    DPushButton *rebootLater = new DPushButton(tr("Restart Later"));
    DPushButton *rebootNow = new DPushButton(tr("Restart Now"));
    connect(rebootLater, SIGNAL(clicked()), this, SLOT(close()));
    connect(rebootNow, SIGNAL(clicked()), bootMaker_, SLOT(reboot()));

    actionLayout_->setMargin(0);
    actionLayout_->setSpacing(20);
    actionLayout_->addWidget(rebootLater);
    actionLayout_->addWidget(rebootNow);
    actionLayout_->addStretch();

    isoLabel_->stop();
}


void DWindowUI::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        pos_ = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}
void DWindowUI::mouseMoveEvent(QMouseEvent *event){
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - pos_);
        event->accept();
    }
}
