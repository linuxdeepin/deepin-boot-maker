#include "dcheckbox.h"

#include <QCheckBox>
#include <QLabel>
#include <QBoxLayout>

DCheckBox::DCheckBox(const QString& text,QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *top = new QHBoxLayout();
    top->setSpacing(0);
    top->setMargin(0);

    checkBox_ = new QCheckBox();
    QString style = "QCheckBox {"
    "    color: white;"
    "    margin: 3px;"
    "    spacing: 5px;"
    "}"
    "QCheckBox::indicator {"
    "    width: 13px;"
    "    height: 13px;"
    "}"
    "QCheckBox::indicator:unchecked {"
    "    image: url(:/ui/images/checkbox_unchecked.png);"
    "}"
    "QCheckBox::indicator:unchecked:hover {"
    "    image: url(:/ui/images/checkbox_unchecked_hover.png);"
    "}"
    "QCheckBox::indicator:unchecked:pressed {"
    "    image: url(:/ui/images/checkbox_unchecked_insensitive.png);"
    "}"
    "QCheckBox::indicator:checked {"
    "    image: url(:/ui/images/checkbox_checked.png);"
    "}"
    "QCheckBox::indicator:checked:hover {"
    "    image: url(:/ui/images/checkbox_checked_hover.png);"
    "}"
    "QCheckBox::indicator:checked:pressed {"
    "    image: url(:/ui/images/checkbox_checked_insensitive.png);"
    "}"
    "QCheckBox::indicator:indeterminate:hover {"
    "    image: url(:/ui/images/checkbox_indeterminate_hover.png);"
    "}"
    "QCheckBox::indicator:indeterminate:pressed {"
    "    image: url(:/ui/images/checkbox_indeterminate_pressed.png);"
    "}";
    checkBox_->setStyleSheet(style);
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

void DCheckBox::click(bool checked) {
    if (checked) {
        setCheckState(Qt::Unchecked);
    } else {
        setCheckState(Qt::Checked);
    }
}
