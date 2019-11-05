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
#include <QLabel>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QListWidget>
#include <DWaterProgress>

#include "suggestbutton.h"
#include "widgetutil.h"
#include "deviceinfoitem.h"
#include "devicelistwidget.h"

#include <bminterface.h>

ProgressView::ProgressView(QWidget *parent) : QWidget(parent)
{
    setObjectName("ProgressView");
    setAutoFillBackground(true);
    QPalette pa;
    pa.setColor(QPalette::Background, QColor(255, 255, 255));
    setPalette(pa);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 9, 0, 0);

    QLabel *m_title = new QLabel(tr("Burning"));
    m_title->setFixedHeight(35);
    QFont ft = m_title->font();
    ft.setPixelSize(24);
    m_title->setFont(ft);
    pa = m_title->palette();
    pa.setColor(QPalette::WindowText, QColor("#001A2E"));
    m_title->setPalette(pa);
//    m_title->setStyleSheet("font-size: 26px;");

    auto waterProgress = new Dtk::Widget::DWaterProgress;
    waterProgress->setFixedSize(100, 100);

    QLabel *m_hitsTitle = new QLabel(tr("Burning, please wait..."));
    m_hitsTitle->setObjectName("ProgressHitsTitle");
    m_hitsTitle->setFixedHeight(25);
    QFont qf;
    qf = m_hitsTitle->font();
    qf.setPixelSize(17);
    qf.setBold(true);
    m_hitsTitle->setFont(qf);
    pa.setColor(QPalette::WindowText, QColor("#001A2E"));
    m_hitsTitle->setPalette(pa);
    m_hitsTitle->setAlignment(Qt::AlignCenter);

    QLabel *m_hits = new QLabel(tr("Do not remove the disk or shut down the computer during the process"));
    m_hits->setObjectName("ProgressHits");
    m_hits->setFixedSize(213, 17);
    m_hits->setWordWrap(true);
    qf = m_hits->font();
    qf.setPixelSize(12);
    m_hits->setFont(qf);
    m_hits->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    pa = m_hits->palette();
    pa.setColor(QPalette::WindowText, QColor("#526A7F"));
    m_hits->setPalette(pa);

    SuggestButton *start = new SuggestButton();
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
