// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "unmountusbview.h"
#include <bminterface.h>
UnmountUsbView::UnmountUsbView(DWidget *parent): DWidget(parent)
{
    setObjectName("safeejectview");
    setAutoFillBackground(true);
    QVBoxLayout *MainLayout = new QVBoxLayout(this);
    MainLayout->setContentsMargins(0, 1, 0, 0);
    MainLayout->setSpacing(0);
    m_spinner = new DSpinner();
    m_spinner->setObjectName("spinner");
    m_spinner->setFixedSize(32, 32);
    m_hitTitle = new DLabel(tr("Verifying data and safely removing the media, please wait..."));
    DPalette pa = DApplicationHelper::instance()->palette(m_hitTitle);
    QBrush brush = DApplicationHelper::instance()->palette(m_hitTitle).text();
    pa.setBrush(DPalette::Text, brush);
    m_hitTitle->setPalette(pa);
    m_hitTitle->setFixedWidth(440);
    m_hitTitle->setFixedHeight(50);
    m_hitTitle->setWordWrap(true);
    m_hitTitle->setAlignment(Qt::AlignTop);
    DFontSizeManager::instance()->bind(m_hitTitle, DFontSizeManager::T5);
    m_hitTitle->setAlignment(Qt::AlignCenter);
    MainLayout->addSpacing(200);
    MainLayout->addWidget(m_spinner, 0, Qt::AlignCenter);
    MainLayout->addSpacing(10);
    MainLayout->addWidget(m_hitTitle, 0, Qt::AlignCenter);
    MainLayout->addStretch();
    DPalette pa1 = DApplicationHelper::instance()->palette(this);
    pa.setBrush(DPalette::Background, pa.color(DPalette::Base));
    this->setPalette(pa1);
    connect(this, &UnmountUsbView::startSpinner, this, [ = ]() {
        m_spinner->start();
    });
    connect(this, &UnmountUsbView::pauseSpinner, this, [ = ]() {
        m_spinner->stop();
    });
}
