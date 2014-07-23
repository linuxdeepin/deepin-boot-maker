#include "dfilechooseinput.h"

#include <QPushButton>
#include <QBoxLayout>
#include <QLineEdit>
#include <QFileDialog>

DFileChooseInput::DFileChooseInput(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout * inputlayout = new QHBoxLayout();
    edit_ = new QLineEdit();
    QString style = ""
    "QLineEdit {"
        "color: #b4b4b4;"
        "font-size: 13px;"
        "border-top: 1px solid #080808;"
        "border-bottom: 1px solid #474747;"
        "border-right: 0px solid #474747;"
        "border-left: 1px solid #080808;"
        "border-top-left-radius: 3px;"
        "border-top-right-radius: 0;"
        "border-bottom-right-radius: 0;"
        "border-bottom-left-radius: 3px;"
        "background: qlineargradient(x1: 1, y1: 0, x2: 0, y2: 0,"
                                "stop: 0 rgba(0, 0, 0, 50%), stop: 1 rgba(0, 0, 0, 40%));"
    "}";
    edit_->setStyleSheet(style);
    edit_->setEnabled(false);
    edit_->setFixedHeight(22);

    QPixmap pixmapfile (":/ui/images/loadfile_normal.png");
    btWidth_ = pixmapfile.size().width() + 4;
    bt_ = new QPushButton(this);
    style = "QPushButton{"
        "border-top: 1px solid #080808;"
        "border-bottom: 1px solid #474747;"
        "border-right: 1px solid #080808;"
        "border-left: 1px solid #474747;"
        "border-top-left-radius: 0;"
        "border-top-right-radius: 3px;"
        "border-bottom-right-radius: 3px;"
        "border-bottom-left-radius: 0px;"
        "background:url(:/ui/images/loadfile_normal.png);"
    "}"
    "QPushButton:hover{"
        "border-top: 1px solid #080808;"
        "border-bottom: 1px solid #474747;"
        "border-right: 1px solid #080808;"
        "border-left: 1px solid #474747;"
        "border-top-left-radius: 0;"
        "border-top-right-radius: 3px;"
        "border-bottom-right-radius: 3px;"
        "border-bottom-left-radius: 0px;"
        "background:url(:/ui/images/loadfile_hover.png);"
    "}"
    "QPushButton:pressed{"
        "border-top: 1px solid #080808;"
        "border-bottom: 1px solid #474747;"
        "border-right: 1px solid #080808;"
        "border-left: 1px solid #474747;"
        "border-top-left-radius: 0;"
        "border-top-right-radius: 3px;"
        "border-bottom-right-radius: 3px;"
        "border-bottom-left-radius: 0px;"
        "background:url(:/ui/images/loadfile_press.png);"
    "}";
    bt_->setStyleSheet(style);
    bt_->setFixedHeight(22);
    bt_->setFixedWidth(pixmapfile.size().width() + 4);

    inputlayout->setSpacing(0);
    inputlayout->setMargin(0);
    inputlayout->addWidget(edit_);
    inputlayout->addWidget(bt_);
    inputlayout->addStretch();

    connect(bt_, SIGNAL(clicked()), this, SLOT(selectFile()));

    this->setLayout(inputlayout);
}

void DFileChooseInput::setFixedWidth(int w) {
    edit_->setFixedWidth(w - btWidth_);
}

void DFileChooseInput::setFixedHeight(int h) {
    edit_->setFixedHeight(h);
    bt_->setFixedHeight(h);
}

void DFileChooseInput::selectFile() {
    QFileDialog fileDlg(this);
    fileDlg.setViewMode( QFileDialog::Detail);
    fileDlg.setFileMode(QFileDialog::ExistingFile);
    fileDlg.setNameFilter("ISO (*.iso);;");
    if (QFileDialog::Accepted == fileDlg.exec()) {
        text_ = fileDlg.selectedFiles().first();
        edit_->setText(text_);
        emit filedSelected(text_);
    }
}
