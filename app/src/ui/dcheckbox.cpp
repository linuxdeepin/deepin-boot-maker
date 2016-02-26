/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dcheckbox.h"

#include <QCheckBox>
#include <QLabel>
#include <QBoxLayout>
#include <QPushButton>
#include <QFontDatabase>
#include <QDebug>

DCheckBox::DCheckBox(const QString& text,QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *top = new QHBoxLayout();
    top->setSpacing(0);
    top->setMargin(0);
    top->addStretch();

    m_styleChecked = "QPushButton{"
        "border-image:url(:/ui/images/checkbox_checked.png);"
        "margin-right: 4px;"
        "margin-top: 2px;"
        #ifdef Q_OS_UNIX
            "margin-top: 6px;"
        #endif
        #ifdef Q_OS_MAC
            "margin-right: 8px;"
            "margin-left: 8px;"
        #endif
    "}"
    "QPushButton:hover{"
        "border-image:url(:/ui/images/checkbox_checked_hover.png);"
    "}"
    "QPushButton:pressed{"
        "border-image:url(:/ui/images/checkbox_checked_insensitive.png);"
    "}";

    m_styleUnchecked = "QPushButton{"
        "border-image:url(:/ui/images/checkbox_unchecked.png);"
        "margin-right: 4px;"
        "margin-top: 2px;"
        #ifdef Q_OS_UNIX
            "margin-top: 6px;"
        #endif
        #ifdef Q_OS_MAC
            "margin-right: 8px;"
            "margin-left: 8px;"
        #endif
    "}"
    "QPushButton:hover{"
        "border-image:url(:/ui/images/checkbox_unchecked_hover.png);"
    "}"
    "QPushButton:pressed{"
        "border-image:url(:/ui/images/checkbox_unchecked_insensitive.png);"
    "}";

    QPixmap checkPixmap(":/ui/images/checkbox_unchecked.png");
    m_checkBox = new QPushButton();
    m_checkBox->setFixedWidth(checkPixmap.size().width() + 4);
    m_checkBox->setFixedHeight(checkPixmap.size().height() + 2);
#ifdef Q_OS_UNIX
    m_checkBox->setFixedHeight(checkPixmap.size().height() + 6);
#endif
#ifdef Q_OS_MAC
    m_checkBox->setFixedWidth(checkPixmap.size().width() + 16);
#endif
    m_checkBox->setStyleSheet(m_styleUnchecked);
    m_checkBox->setFocusPolicy(Qt::NoFocus);
    top->addWidget(m_checkBox);
    top->setAlignment(m_checkBox, Qt::AlignTop);
    m_indicatorSize = 13;
    top->addSpacing(4);
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
void DCheckBox::setCbk(bool checked) {
        m_checkBox->hide();
    if (!checked) {
        m_checkBox->setStyleSheet(m_styleUnchecked);
        m_checkState = Qt::Unchecked;
    } else {
        m_checkBox->setStyleSheet(m_styleChecked);
        m_checkState = Qt::Checked;
    }
    m_checkBox->show();

    m_checkBox->update();
}
void DCheckBox::setChecked(bool checked){
    if (!checked) {
        m_checkBox->setStyleSheet(m_styleUnchecked);
        m_checkState = Qt::Unchecked;
    } else {
        m_checkBox->setStyleSheet(m_styleChecked);
        m_checkState = Qt::Checked;
    }
    m_checkBox->repaint();
}

void DCheckBox::setDisabled(bool disable) {
    m_checkBox->setDisabled(disable);
}
