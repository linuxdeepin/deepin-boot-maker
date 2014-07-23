#include "dcheckbox.h"

#include <QCheckBox>
#include <QLabel>
#include <QBoxLayout>
#include <QPushButton>

DCheckBox::DCheckBox(const QString& text,QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *top = new QHBoxLayout();
    top->setSpacing(0);
    top->setMargin(0);

    styleChecked_ = "QPushButton{"
        "border-image:url(:/ui/images/checkbox_checked.png);"
        "margin-top: 2px;"
        "margin-right: 4px;"
    "}"
    "QPushButton:hover{"
        "border-image:url(:/ui/images/checkbox_checked_hover.png);"
        "margin-top: 2px;"
        "margin-right: 4px;"
    "}"
    "QPushButton:pressed{"
        "border-image:url(:/ui/images/checkbox_checked_insensitive.png);"
        "margin-top: 2px;"
        "margin-right: 4px;"
    "}";

    styleUnchecked_ = "QPushButton{"
        "border-image:url(:/ui/images/checkbox_unchecked.png);"
        "margin-top: 2px;"
        "margin-right: 4px;"
    "}"
    "QPushButton:hover{"
        "border-image:url(:/ui/images/checkbox_unchecked_hover.png);"
        "margin-top: 2px;"
        "margin-right: 4px;"
    "}"
    "QPushButton:pressed{"
        "border-image:url(:/ui/images/checkbox_unchecked_insensitive.png);"
        "margin-top: 2px;"
        "margin-right: 4px;"
    "}";

    QPixmap checkPixmap(":/ui/images/checkbox_unchecked.png");
    checkBox_ = new QPushButton();
    checkBox_->setFixedWidth(checkPixmap.size().width() + 4);
    checkBox_->setFixedHeight(checkPixmap.size().height() + 2);
    checkBox_->setStyleSheet(styleUnchecked_);
    top->addWidget(checkBox_);
    top->setAlignment(checkBox_, Qt::AlignTop);
    indicatorSize_ = 13;

    label_ = new QLabel(text);
    label_->setWordWrap(true);
    top->addWidget(label_);
    top->addStretch();
    this->setLayout(top);

    connect(checkBox_, SIGNAL(clicked()), this, SLOT(click()));
}

void DCheckBox::setFixedWidth(int w) {
    label_->setFixedWidth(w - indicatorSize_);
}

void DCheckBox::click() {
    if (Qt::Checked == checkState_) {
        checkBox_->setStyleSheet(styleUnchecked_);
        setCheckState(Qt::Unchecked);
    } else {
        checkBox_->setStyleSheet(styleChecked_);
        setCheckState(Qt::Checked);
    }
    emit clicked();
}

void DCheckBox::setChecked(bool checked){
    if (!checked) {
        checkBox_->setStyleSheet(styleUnchecked_);
        setCheckState(Qt::Unchecked);
    } else {
        checkBox_->setStyleSheet(styleChecked_);
        setCheckState(Qt::Checked);
    }
}
