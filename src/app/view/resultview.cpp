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

#include "resultview.h"
#include "widgetutil.h"
#include "deviceinfoitem.h"
#include "devicelistwidget.h"
#include "dwaterprogress.h"
#include <bminterface.h>
#include <backend/bmhandler.h>

#include <DListWidget>
#include <DApplication>
#include <DCheckBox>
#include <DLog>
#include <DApplicationHelper>
#include <DFontSizeManager>

#include <QDebug>
#include <QUrl>
#include <QVBoxLayout>
#include <QProcess>
#include <QDesktopServices>
#include <QFontDatabase>
#include <DSysInfo>
#include <QDBusInterface>
#include <QDBusAbstractInterface>
#include <QDBusError>
#include <QDBusReply>

ResultView::ResultView(DWidget *parent) : DWidget(parent)
{
    setObjectName("ResultView");
    setAutoFillBackground(true);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 1, 0, 0);

    m_title = new DLabel();
    m_title->setAccessibleName("resultWidget_title");
    DPalette pa = DApplicationHelper::instance()->palette(m_title);
    QBrush brush = DApplicationHelper::instance()->palette(m_title).text();
    pa.setBrush(DPalette::Text, brush);
    m_title->setPalette(pa);
    DFontSizeManager::instance()->bind(m_title, DFontSizeManager::T3);

    m_resultIcon = new DLabel();
    m_resultIcon->setAccessibleName("resultWidget_resultIconLabel");
    m_resultIcon->setObjectName("ResultIcon");
    m_resultIcon->setPixmap(WidgetUtil::getDpiPixmap(":/theme/light/image/success.svg", this));

    m_hitsTitle = new DLabel(tr("Successful"));
    m_hitsTitle->setAccessibleName("resultWidget_hitsTitleLabel");
    m_hitsTitle->setObjectName("ResulteHitsTitle");
    pa = DApplicationHelper::instance()->palette(m_hitsTitle);
    brush = DApplicationHelper::instance()->palette(m_hitsTitle).text();
    pa.setBrush(DPalette::Text, brush);
    m_hitsTitle->setPalette(pa);
    m_hitsTitle->setAlignment(Qt::AlignCenter);
    m_hitsTitle->setContentsMargins(20, 0, 20, 0);

    DFontSizeManager::instance()->bind(m_hitsTitle, DFontSizeManager::T5);

    m_logHits = new DLabel(/*hitsFormat.arg(log.arg(tagBegin).arg(tagEnd))*/);
    m_logHits->setAccessibleName("resultWidget_logHitsLabel");
    m_logHits->setObjectName("ResultErrorDescription");
    m_logHits->setContentsMargins(20, 0, 20, 0);
    m_logHits->setWordWrap(true);
    m_logHits->setAlignment(Qt::AlignHCenter);
    DFontSizeManager::instance()->bind(m_logHits, DFontSizeManager::T8);


    m_rebootLater = new DPushButton();
    m_rebootLater->setAccessibleName("resultWidget_rebootLaterButton");
    m_rebootLater->setFocusPolicy(Qt::NoFocus);
    m_rebootLater->setFixedSize(310, 36);
    m_rebootLater->setObjectName("RebootLater");
    m_rebootLater->setText(tr("Done", "button"));
    m_rebootLater->setProperty("normal", true);
    DFontSizeManager::instance()->bind(m_rebootLater, DFontSizeManager::T6);

    m_rebootNow = new DPushButton();
    m_rebootNow->setAccessibleName("resultWidget_rebootNowButton");
    m_rebootNow->setFocusPolicy(Qt::NoFocus);
    m_rebootNow->setFixedSize(310, 36);
    m_rebootNow->setObjectName("RebootLater");
    m_rebootNow->setText(tr("Reboot now"));
    DFontSizeManager::instance()->bind(m_rebootNow, DFontSizeManager::T6);

    mainLayout->addWidget(m_title, 0, Qt::AlignHCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(m_resultIcon, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(11);
    mainLayout->addWidget(m_hitsTitle);
    mainLayout->addWidget(m_logHits);
    mainLayout->addStretch();
    mainLayout->addWidget(m_rebootLater, 0, Qt::AlignHCenter|Qt::AlignBottom);

    connect(m_rebootNow, &DPushButton::clicked,
    this, [ = ]() {
        BMInterface::instance()->reboot();
    });

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
    this, [ = ] {
        DPalette pa;
        DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
        if (themeType == DGuiApplicationHelper::LightType)
        {
            pa = palette();
            pa.setColor(DPalette::Background, QColor(255, 255, 255));
            setPalette(pa);

            pa = m_hitsTitle->palette();
            pa.setColor(DPalette::WindowText, QColor("#001A2E"));
            m_hitsTitle->setPalette(pa);
            pa = m_logHits->palette();
            pa.setColor(DPalette::WindowText, QColor("#526A7F"));
            m_logHits->setPalette(pa);
        } else if (themeType == DGuiApplicationHelper::DarkType) {
            pa = palette();
            pa.setColor(DPalette::Background, QColor("#292929"));
            setPalette(pa);
            pa = m_hitsTitle->palette();
            pa.setColor(DPalette::WindowText, QColor("#C0C6D4"));
            m_hitsTitle->setPalette(pa);
            pa = m_logHits->palette();
            pa.setColor(DPalette::WindowText, QColor("#6D7C88"));
            m_logHits->setPalette(pa);
        }
    });

    emit DGuiApplicationHelper::instance()->themeTypeChanged(DGuiApplicationHelper::instance()->themeType());

}

void ResultView::updateResult(quint32 error, const QString &/*title*/, const QString &/*description*/)
{
    auto errorType = static_cast<BMHandler::ErrorType>(error);

    switch (errorType) {
    case BMHandler::NoError:
        m_rebootLater->disconnect();
        connect(m_rebootLater, &DPushButton::clicked,
        this, [ = ]() {
            qApp->exit(0);
        });
        return;
    case BMHandler::SyscExecFailed:
    case BMHandler::ErrorType::InstallBootloaderFailed:
        if (DTK_NAMESPACE::DCORE_NAMESPACE::DSysInfo::uosEditionType() != DTK_NAMESPACE::DCORE_NAMESPACE::DSysInfo::UosProfessional) {
            m_logHits->setText(tr("The error log will be uploaded automatically with the feedback. We cannot improve without your feedback"));
            m_rebootLater->setText(tr("Submit Feedback"));
            m_logHits->adjustSize();
            m_rebootLater->disconnect();
            connect(m_rebootLater, &DPushButton::clicked,
            this, [ = ]() {
                // FIXME: call feedback 非专业版保持链接进社区
                if (DTK_NAMESPACE::DCORE_NAMESPACE::DSysInfo::uosEditionType() == DTK_NAMESPACE::DCORE_NAMESPACE::DSysInfo::UosCommunity) {
                    QDesktopServices::openUrl(QString("https://bbs.deepin.org/post/209286"));
                }
                else if(DTK_NAMESPACE::DCORE_NAMESPACE::DSysInfo::uosEditionType() == DTK_NAMESPACE::DCORE_NAMESPACE::DSysInfo::UosHome) {
                    QDesktopServices::openUrl(QString("https://bbs.chinauos.com/post/4838?id=4838&type_id=5&forum_name=%E6%A1%8C%E9%9D%A2%E4%B8%AA%E4%BA%BA%E7%89%88"));
                }
                else if((DTK_NAMESPACE::DCORE_NAMESPACE::DSysInfo::uosEditionType() == DTK_NAMESPACE::DCORE_NAMESPACE::DSysInfo::UosEnterprise) ||
                (DTK_NAMESPACE::DCORE_NAMESPACE::DSysInfo::uosEditionType() == DTK_NAMESPACE::DCORE_NAMESPACE::DSysInfo::UosEnterprise))
                {
                    QDesktopServices::openUrl(QString("https://bbs.chinauos.com/zh/post/5609"));
                }
                else {
                    QProcess::startDetached("deepin-feedback");
                }
            });

        } else {
            m_rebootLater->setText(tr("After-Sale Services"));
            m_logHits->adjustSize();
            m_rebootLater->disconnect();
            connect(m_rebootLater, &DPushButton::clicked,
            this, [ = ]() {
                // FIXME: call service-support  fix bug 19711 专业版不再调用deepin-feedback链接进社区，而是调用服务与支持客户端
                QDBusInterface syssupport("com.deepin.dde.ServiceAndSupport", "/com/deepin/dde/ServiceAndSupport", "com.deepin.dde.ServiceAndSupport");
                syssupport.call("ServiceSession", 2);
            });
        }
        break;
    default:
        m_logHits->setText(BMHandler::errorString(errorType));
        m_rebootLater->setText(tr("Close", "button"));
        m_rebootLater->disconnect();
        connect(m_rebootLater, &DPushButton::clicked,
        this, [ = ]() {
            qApp->exit(0);
        });
        break;
    }
    m_hitsTitle->setText(tr("Sorry, process failed"));
//    m_title->setText(tr("Process failed"));
    m_resultIcon->setPixmap(WidgetUtil::getDpiPixmap(":/theme/light/image/fail.svg", this));
    m_logHits->show();
    m_rebootNow->hide();
}

void ResultView::onLogLinkActivated(const QString &link)
{
    if (link == "#show_log") {
        QDesktopServices::openUrl(QUrl::fromLocalFile(Dtk::Core::DLogManager::getlogFilePath()));
    }
}
