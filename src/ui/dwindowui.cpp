#include "dwindowui.h"

#include "dcombobox.h"
#include "dfilechooseinput.h"
#include "dcheckbox.h"
#include "dpushbutton.h"

#include "../bootmaker.h"

#include <QPainter>
#include <QLayout>
#include <QLabel>
#include <QMovie>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QListView>
#include <QTimer>

DWindowUI::DWindowUI(QWidget *parent) :
    QMainWindow(parent)
{
    resize(680, 440);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    QString style = "DWindowUI {"
            "margin : 1px"
            "}";
    setStyleSheet(style);

    bootMaker_ = new BootMaker(this);
    InitUI();
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
    QWidget *window = new QWidget();
    window->setLayout(top);
    setCentralWidget(window);

    QTimer *usbtimer=new QTimer(this);
    usbtimer->setInterval(3000);
    connect(usbtimer, SIGNAL(timeout()), this, SLOT(refrshUsbDriverList()));
    usbtimer->start();
}

void DWindowUI::AddProcessUI(QBoxLayout * top) {
    QVBoxLayout *process = new QVBoxLayout();

    QSpacerItem *sp = new QSpacerItem(64,64,QSizePolicy::Minimum, QSizePolicy::Expanding);
    process->addSpacerItem(sp);

    isoLabel =new QLabel();
    isoLabel->setPixmap(QPixmap(":/image/iso-inactive.png"));
    process->addWidget(isoLabel);
    process->setAlignment(isoLabel, Qt::AlignCenter);

    processLabel =new QLabel();
    QMovie *processMovie = new QMovie(":/image/process-active.gif");
    processLabel->setMovie(processMovie);
    process->addWidget(processLabel);
    process->setAlignment(processLabel, Qt::AlignCenter);
    processMovie->start();

    usbLabel =new QLabel();
    usbLabel->setPixmap(QPixmap(":/image/usb-inactive.png"));
    process->addWidget(usbLabel);
    process->setAlignment(usbLabel, Qt::AlignCenter);

    process->addSpacerItem(sp);

    top->addLayout(process);

}

void DWindowUI::AddOptionUI(QBoxLayout *top) {
    QVBoxLayout *option = new QVBoxLayout();
    option->setSpacing(8);

    const int labelMaxWidth =380;

    //add close button
    QHBoxLayout *btlayout = new QHBoxLayout();
    QPushButton *closeBt = new QPushButton();
    QString style = "QPushButton{"
        "background:url(:/image/window_close_normal.png);"
        "border:0px;"
    "}"
    "QPushButton:hover{"
        "background:url(:/image/window_close_hover.png);"
        "border:0px;"
    "}"
    "QPushButton:pressed{"
        "background:url(:/image/window_close_press.png);"
        "border:0px;"
    "}";

    closeBt->setStyleSheet(style);
    QPixmap pixmap (":/image/window_close_normal.png");
    closeBt->setFixedSize(pixmap.size());
    connect(closeBt, SIGNAL(clicked()), this, SLOT(close()));
    btlayout->setMargin(3);
    btlayout->addWidget(closeBt);
    btlayout->setAlignment(closeBt, Qt::AlignRight);

    //Add Logo
    QHBoxLayout *logolayout = new QHBoxLayout();
    QLabel *logolabel = new QLabel();
    logolabel->setPixmap(QPixmap(":/image/logo.png"));

    logolayout->addSpacing(50);
    logolayout->addWidget(logolabel);

    QLabel *versionlabel = new QLabel(tr("<p style='color:white; font-size:10px;'>0.995</p>"));
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

    //Add select iso hint
    QLabel *selectISO = new QLabel(tr("<p style='color:white; font-size:12px;'>Select the ISO File:</p>"));
    selectISO->setFixedWidth(labelMaxWidth);
    option->addWidget(selectISO);

    isoFile = new DFileChooseInput();
    isoFile->setFixedHeight(22);
    isoFile->setFixedWidth(210);

    option->addWidget(isoFile);

    //Add select usb driver hint
    QLabel *selectUSB = new QLabel(tr("<p style='color:white; font-size:12px;'>Select the USB Flash Drive:</p>"));
    selectISO->setFixedWidth(labelMaxWidth);
    option->addWidget(selectUSB);

    usbDriver = new DComboBox();
    usbDriver->setFixedWidth(210);

    option->addWidget(usbDriver);

    formatDisk = new DCheckBox("<p style='color:white; font-size:12px;'>Format USB flash disk before installation to improve the making success rate.</p>");
    formatDisk->setFixedWidth(labelMaxWidth);
    option->addWidget(formatDisk);

    bisoMode = new DCheckBox("<p style='color:white; font-size:12px;'>Support BIOS. Unselect here.</p>");
    bisoMode->setFixedWidth(labelMaxWidth);
    option->addWidget(bisoMode);

    option->addSpacing(15);
    actionlayout = new QHBoxLayout();
    startBt = new DPushButton(tr("Start"));
    startBt->setFixedWidth(100);
    actionlayout->addSpacing(50);
    actionlayout->addWidget(startBt);
    actionlayout->addStretch();
    actionlayout->setAlignment(startBt, Qt::AlignCenter);
    option->addLayout(actionlayout);

    connect(startBt, SIGNAL(clicked()), this, SLOT(start()));

    option->addStretch();
    top->addLayout(option);
}

void DWindowUI::start() {
    QString isoPath = isoFile->text();
    QString usbDev = usbDriver->currentText();
    bool format = formatDisk->checked();
    bool bios = bisoMode->checked();

    if (0 == bootMaker_->start(isoPath, usbDev, bios, format)) {
        //Next
        startBt->setVisible(false);
    }
}

void DWindowUI::refrshUsbDriverList() {
    QString curText = usbDriver->currentText();
    usbDriver->clear();
    usbDriver->addItem(curText);
    QStringList list = bootMaker_->listUsbDrives();
    usbDriver->addItems(list);

    usbDriver->removeItem(0);
    if (0 == list.size()) {
        usbDriver->setCurrentText("");
    } else {
        if (list.contains(curText)) {
            //usbDriver->setCurrentText(curText);
        } else {
            usbDriver->setCurrentText(list.first());
        }
    }
}
