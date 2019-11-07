/*
 * Copyright (C) 2017 ~ 2018 Wuhan Deepin Technology Co., Ltd.
 *
 * Author:     Iceyer <me@iceyer.net>
 *
 * Maintainer: Iceyer <me@iceyer.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "progressview.h"

#include <QDebug>
#include <QVBoxLayout>
#include <DCheckBox>
#include <DLabel>
#include <DListWidget>
#include <DWaterProgress>
#include <DPushButton>
#include <DApplicationHelper>

//#include "suggestbutton.h"
#include "widgetutil.h"
#include "deviceinfoitem.h"
#include "devicelistwidget.h"

#include <bminterface.h>

DWIDGET_USE_NAMESPACE

ProgressView::ProgressView(DWidget *parent) : DWidget(parent)
{
    setObjectName("ProgressView");
    setAutoFillBackground(true);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 9, 0, 0);

    DLabel *m_title = new DLabel(tr("Burning"));
    m_title->setFixedHeight(35);
    QFont ft = m_title->font();
    ft.setFamily("SourceHanSansSC-Medium");
    ft.setPixelSize(24);
    m_title->setFont(ft);
//    m_title->setStyleSheet("font-size: 26px;");

    auto waterProgress = new Dtk::Widget::DWaterProgress;
    waterProgress->setFixedSize(100, 100);

    DLabel *m_hitsTitle = new DLabel(tr("Burning, please wait..."));
    m_hitsTitle->setObjectName("ProgressHitsTitle");
    m_hitsTitle->setFixedHeight(25);
    QFont qf;
    qf = m_hitsTitle->font();
    qf.setFamily("SourceHanSansSC-Bold");
    qf.setPixelSize(17);
    qf.setBold(true);
    m_hitsTitle->setFont(qf);
    m_hitsTitle->setAlignment(Qt::AlignCenter);

    DLabel *m_hits = new DLabel(tr("Do not remove the disk or shut down the computer during the process"));
    m_hits->setObjectName("ProgressHits");
    m_hits->setFixedSize(213, 17);
    m_hits->setWordWrap(true);
    qf = m_hits->font();
    qf.setPixelSize(12);
    m_hits->setFont(qf);
    m_hits->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

//    SuggestButton *start = new SuggestButton();
    DPushButton *start = new DPushButton();
    start->setFocusPolicy(Qt::NoFocus);
    start->setFixedSize(310, 36);
    start->setObjectName("ProgressCancel");
    start->setText(tr("Cancel"));

    mainLayout->addWidget(m_title, 0, Qt::AlignCenter);
    mainLayout->addSpacing(102);
    mainLayout->addWidget(waterProgress, 0, Qt::AlignCenter);
    mainLayout->addSpacing(30);
    mainLayout->addWidget(m_hitsTitle, 0, Qt::AlignCenter);
    mainLayout->addSpacing(6);
    mainLayout->addWidget(m_hits, 0, Qt::AlignCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(start, 0, Qt::AlignCenter);

    waterProgress->setValue(0);
    waterProgress->start();

//    this->setStyleSheet(WidgetUtil::getQss(":/theme/light/ProgressView.theme"));
//    this->setStyleSheet("#ProgressHits{line-height: 1.67;}");

//    connect(start, &SuggestButton::clicked, this, &ProgressView::testCancel);
    start->hide();


    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
    this, [ = ] {
        DPalette pa;
        DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
        if (themeType == DGuiApplicationHelper::LightType)
        {
            pa = palette();
            pa.setColor(DPalette::Background, QColor(255, 255, 255));
            setPalette(pa);
            pa = m_title->palette();
            pa.setColor(DPalette::Text, QColor("#001A2E"));
            m_title->setPalette(pa);
            pa = m_hitsTitle->palette();
            pa.setColor(DPalette::Text, QColor("#001A2E"));
            m_hitsTitle->setPalette(pa);
            pa = m_hits->palette();
            pa.setColor(DPalette::Text, QColor("#526A7F"));
            m_hits->setPalette(pa);
        } else if (themeType == DGuiApplicationHelper::DarkType)
        {
            pa = palette();
            pa.setColor(DPalette::Background, QColor("#252525"));
            setPalette(pa);
            pa = m_title->palette();
            pa.setColor(DPalette::Text, QColor("#C0C6D4"));
            m_title->setPalette(pa);
            pa = m_hitsTitle->palette();
            pa.setColor(DPalette::Text, QColor("#C0C6D4"));
            m_hitsTitle->setPalette(pa);
            pa = m_hits->palette();
            pa.setColor(DPalette::Text, QColor("#6D7C88"));
            m_hits->setPalette(pa);
        }
    });

    emit DGuiApplicationHelper::instance()->themeTypeChanged(DGuiApplicationHelper::instance()->themeType());

    connect(BMInterface::instance(), &BMInterface::reportProgress,
    this, [ = ](quint32 current, quint32 error, const QString & title, const QString & description) {
        qDebug() << error << current << title << description;
        waterProgress->setValue(static_cast<int>(current));
        if (current >= 100) {
            qDebug() << "finish" << current << error;
            emit finish(error, title, description);
        }
    });
}
