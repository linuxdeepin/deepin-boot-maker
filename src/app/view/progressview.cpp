// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "progressview.h"
#include "widgetutil.h"
#include "deviceinfoitem.h"
#include "devicelistwidget.h"
#include <bminterface.h>

#include <DCheckBox>
#include <DLabel>
#include <DListWidget>
#include <DPushButton>
#include <DGuiApplicationHelper>
#include <DTipLabel>
#include <DFontSizeManager>

#include <QDebug>
#include <QVBoxLayout>
#include <QFontDatabase>
#include <QTimerEvent>

DWIDGET_USE_NAMESPACE

ProgressView::ProgressView(DWidget *parent) : DWidget(parent)
 ,m_iInterval(-1)
{
    setObjectName("ProgressView");
    setAutoFillBackground(true);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 1, 0, 0);

    DLabel *m_title = new DLabel(tr("Burning"));
    m_title->setAccessibleName("progressWidget_title");
    QPalette pa = m_title->palette();
    QBrush brush = pa.text();
    pa.setBrush(DPalette::Text, brush);
    m_title->setPalette(pa);
    DFontSizeManager::instance()->bind(m_title, DFontSizeManager::T3);

    m_waterProgress = new Dtk::Widget::DWaterProgress;
    m_waterProgress->setAccessibleName("progressWidget_waterProgress");
    m_waterProgress->setFixedSize(100, 100);

    DLabel *m_hitsTitle = new DLabel(tr("Burning, please wait..."));
    m_hitsTitle->setAccessibleName("progressWidget_hitsTitleLabel");
    m_hitsTitle->setObjectName("ProgressHitsTitle");
    DFontSizeManager::instance()->bind(m_hitsTitle, DFontSizeManager::T5);
    m_hitsTitle->setAlignment(Qt::AlignCenter);
    m_hitsTitle->setWordWrap(true);
    m_hitsTitle->setContentsMargins(20, 0, 20, 0);

    DTipLabel *m_hits = new DTipLabel(tr("Do not remove the disk or shut down the computer during the process"));
    m_hits->setAccessibleName("progressWidget_hitsTipLabel");
    m_hits->setObjectName("ProgressHits");
    m_hits->setWordWrap(true);
    m_hits->setAlignment(Qt::AlignCenter);
    m_hits->setContentsMargins(20, 0, 20, 0);
    DFontSizeManager::instance()->bind(m_hits, DFontSizeManager::T8);
    m_hits->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    DPushButton *start = new DPushButton();
    start->setFocusPolicy(Qt::NoFocus);
    start->setAccessibleName("progressWidget_cancelButton");
    start->setFixedSize(310, 36);
    start->setObjectName("ProgressCancel");
    start->setText(tr("Cancel", "button"));
    DFontSizeManager::instance()->bind(start, DFontSizeManager::T6);

    mainLayout->addWidget(m_title, 0, Qt::AlignHCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(m_waterProgress, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(26);
    mainLayout->addWidget(m_hitsTitle);
    mainLayout->addWidget(m_hits);
    mainLayout->addWidget(start, 0, Qt::AlignHCenter);
    mainLayout->addStretch();

    m_waterProgress->setValue(0);
    m_waterProgress->start();

    start->hide();


    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
    this, [ = ] {
        DPalette pa;
        DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
        if (themeType == DGuiApplicationHelper::LightType)
        {
            pa = palette();
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            pa.setColor(DPalette::Background, QColor(255, 255, 255));
#else
            pa.setColor(DPalette::Window, QColor(255, 255, 255));
#endif
            setPalette(pa);

            pa = m_hitsTitle->palette();
            pa.setColor(DPalette::WindowText, QColor("#001A2E"));
            m_hitsTitle->setPalette(pa);
        } else if (themeType == DGuiApplicationHelper::DarkType)
        {
            pa = palette(); 
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            pa.setColor(DPalette::Background, QColor("#292929"));
#else
            pa.setColor(DPalette::Window, QColor("#292929"));
#endif
            setPalette(pa);
            pa = m_hitsTitle->palette();
            pa.setColor(DPalette::WindowText, QColor("#C0C6D4"));
            m_hitsTitle->setPalette(pa);
        }
    });

    emit DGuiApplicationHelper::instance()->themeTypeChanged(DGuiApplicationHelper::instance()->themeType());
    
    // TODO dtk6 中BMInterface::instance()被屏蔽了
    connect(&BMInterface::ref(), &BMInterface::reportProgress,
    this, [ = ](quint32 current, quint32 error, const QString & title, const QString & description) {
        qDebug() << error << current << title << description;

        if (current <= 70) {
            m_waterProgress->setValue(static_cast<int>(current));
        }
        else if (current <= 100) {
            if(m_iInterval == -1) {
                m_iInterval = QObject::startTimer(10000);
            }
        }
        else {
            qDebug() << "finish" << current << error;

            if(m_iInterval != -1) {
                QObject::killTimer(m_iInterval);
                m_iInterval = -1;
            }

            emit finish(current, error, title, description);
        }
    });

}

void ProgressView::timerEvent(QTimerEvent *event)
{
    if (m_iInterval == event->timerId()) {
        int iVal = m_waterProgress->value();

        if (iVal < 99) {
            m_waterProgress->setValue(m_waterProgress->value() + 1);
        }
    }

    DWidget::timerEvent(event);
}
