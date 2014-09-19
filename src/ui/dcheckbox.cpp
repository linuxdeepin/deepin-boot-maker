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
    top->addStretch();

    m_styleChecked = "QPushButton{"
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

    m_styleUnchecked = "QPushButton{"
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
    m_checkBox = new QPushButton();
    m_checkBox->setFixedWidth(checkPixmap.size().width() + 4);
    m_checkBox->setFixedHeight(checkPixmap.size().height() + 2);
    m_checkBox->setStyleSheet(m_styleUnchecked);
    m_checkBox->setFocusPolicy(Qt::NoFocus);
    top->addWidget(m_checkBox);
    top->setAlignment(m_checkBox, Qt::AlignTop);
    m_indicatorSize = 13;

    m_label = new QLabel(text);
    m_label->setWordWrap(true);
    top->addWidget(m_label);
    top->setAlignment(m_label, Qt::AlignTop);
    top->addStretch();
    this->setLayout(top);

    connect(m_checkBox, SIGNAL(clicked()), this, SLOT(click()));
}

void DCheckBox::setFixedWidth(int w) {
    m_label->setFixedWidth(w - m_indicatorSize);
}

void DCheckBox::click() {
    if (Qt::Checked == m_checkState) {
        m_checkBox->setStyleSheet(m_styleUnchecked);
        setCheckState(Qt::Unchecked);
    } else {
        m_checkBox->setStyleSheet(m_styleChecked);
        setCheckState(Qt::Checked);
    }
    emit clicked();
}

void DCheckBox::setChecked(bool checked){
    if (!checked) {
        m_checkBox->setStyleSheet(m_styleUnchecked);
        setCheckState(Qt::Unchecked);
    } else {
        m_checkBox->setStyleSheet(m_styleChecked);
        setCheckState(Qt::Checked);
    }
}

void DCheckBox::setDisabled(bool disable) {
    m_checkBox->setDisabled(disable);
}
