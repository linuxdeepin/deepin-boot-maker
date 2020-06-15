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
ResultView::ResultView(DWidget *parent) : DWidget(parent)
{
    setObjectName("ResultView");
    setAutoFillBackground(true);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 1, 0, 0);
    mainLayout->setSpacing(0);

//    int lcdFontId = QFontDatabase::addApplicationFont(":/theme/SourceHanSansSC-Medium.ttf");
//    int lcdFontId1 = QFontDatabase::addApplicationFont(":/theme/SourceHanSansSC-Bold.ttf");
//    QStringList m_fontList;
//    m_fontList.clear();
//    if (lcdFontId != -1) {
//        m_fontList << QFontDatabase::applicationFontFamilies(lcdFontId);
//    }
//    if (lcdFontId != -1) {
//        m_fontList << QFontDatabase::applicationFontFamilies(lcdFontId1);
//    }

    m_title = new DLabel(tr("Successful"));
    DPalette pa = DApplicationHelper::instance()->palette(m_title);
    QBrush brush = DApplicationHelper::instance()->palette(m_title).text();
    pa.setBrush(DPalette::Text, brush);
    m_title->setPalette(pa);
    m_title->setFixedHeight(36);
    DFontSizeManager::instance()->bind(m_title, DFontSizeManager::T3);
//    QFont qf = m_title->font();
//    if (m_fontList.size() > 0)
//        qf.setFamily(m_fontList.at(0));
//    qf.setPixelSize(24);
//    m_title->setFont(qf);

    m_resultIcon = new DLabel();
    m_resultIcon->setObjectName("ResultIcon");
    m_resultIcon->setPixmap(WidgetUtil::getDpiPixmap(":/theme/light/image/success.svg", this));

    m_hitsTitle = new DLabel();
    m_hitsTitle->setObjectName("ResulteHitsTitle");
    pa = DApplicationHelper::instance()->palette(m_hitsTitle);
    brush = DApplicationHelper::instance()->palette(m_hitsTitle).text();
    pa.setBrush(DPalette::Text, brush);
    m_hitsTitle->setPalette(pa);
    m_hitsTitle->setFixedWidth(340);
    m_hitsTitle->setFixedHeight(25);

    DFontSizeManager::instance()->bind(m_hitsTitle, DFontSizeManager::T5);
//    qf = m_hitsTitle->font();
//    qf.setPixelSize(17);
//    if (m_fontList.size() > 1)
//        qf.setFamily(m_fontList.at(1));
////    qf.setBold(true);
//    m_hitsTitle->setFont(qf);
    m_hitsTitle->setAlignment(Qt::AlignCenter);

//    QString hitsFormat = "<a style='color:#b4b4b4; font-size:11px'>%1</a>";
//    QString tagBegin = "<a href='#show_log'><span style='text-decoration: underline; color:#1B85ff;'>";
//    QString tagEnd = "</span></a>";
//    QString log = tr("Installation logs are stored in %1HERE%2, you can upload to forum to help us solve your problem.");
    m_logHits = new DLabel(/*hitsFormat.arg(log.arg(tagBegin).arg(tagEnd))*/);
//    m_logHits = new DTipLabel(/*hitsFormat.arg(log.arg(tagBegin).arg(tagEnd))*/);   如果不注释这一行，Windows上无法编译
    m_logHits->setObjectName("ResultErrorDescription");
    m_logHits->setWordWrap(true);
    m_logHits->setFixedWidth(400);
    m_logHits->setFixedHeight(100);
    m_logHits->setMargin(9);
//    connect(m_logHits, &QLabel::linkActivated, this, &ResultView::onLogLinkActivated);
//    m_logHits->setOpenExternalLinks(false);
//    m_logHits->hide();
    DFontSizeManager::instance()->bind(m_logHits, DFontSizeManager::T8);
//    qf = m_logHits->font();
//    if (m_fontList.size() > 0)
//        qf.setFamily(m_fontList.at(0));
//    qf.setPixelSize(12);
//    m_logHits->setFont(qf);
//    DPalette pa = DApplicationHelper::instance()->palette(m_logHits);
//    QBrush brush = DApplicationHelper::instance()->palette(m_logHits).textTips();
//    pa.setBrush(DPalette::WindowText, brush);
//    m_logHits->setPalette(pa);
    m_logHits->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

//    m_rebootLater = new SuggestButton();
    m_rebootLater = new DPushButton();
    m_rebootLater->setFocusPolicy(Qt::NoFocus);
    m_rebootLater->setFixedSize(310, 36);
    m_rebootLater->setObjectName("RebootLater");
    m_rebootLater->setText(tr("Done"));
    m_rebootLater->setProperty("normal", true);
    DFontSizeManager::instance()->bind(m_rebootLater, DFontSizeManager::T6);
//    qf = m_rebootLater->font();
//    if (m_fontList.size() > 0)
//        qf.setFamily(m_fontList.at(0));
//    qf.setPixelSize(14);
//    m_rebootLater->setFont(qf);
    /*
    rebootLater->style()->unpolish(rebootLater);
    rebootLater->style()->polish(rebootLater);*/

//    m_rebootNow = new SuggestButton();
    m_rebootNow = new DPushButton();
    m_rebootNow->setFocusPolicy(Qt::NoFocus);
    m_rebootNow->setFixedSize(310, 36);
    m_rebootNow->setObjectName("RebootLater");
    m_rebootNow->setText(tr("Reboot now"));
    DFontSizeManager::instance()->bind(m_rebootNow, DFontSizeManager::T6);
//    qf = m_rebootNow->font();
//    qf.setFamily("SourceHanSansSC-Medium");
//    qf.setPixelSize(14);
//    m_rebootNow->setFont(qf);

    mainLayout->addWidget(m_title, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(87);
    mainLayout->addWidget(m_resultIcon, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(11);
    mainLayout->addWidget(m_hitsTitle, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(0);
    mainLayout->addWidget(m_logHits, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(34);
    mainLayout->addWidget(m_rebootLater, 0, Qt::AlignHCenter);
    mainLayout->addStretch();
//    mainLayout->addSpacing(15);
//    m_rebootNow->hide();
//    mainLayout->addWidget(m_rebootNow, 0, Qt::AlignCenter);


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
//            pa = m_title->palette();
//            pa.setColor(DPalette::WindowText, QColor("#414D68"));
//            m_title->setPalette(pa);
//            pa = m_hitsTitle->palette();
//            pa.setColor(DPalette::WindowText, QColor("#001A2E"));
//            m_hitsTitle->setPalette(pa);
//            pa = m_logHits->palette();
//            pa.setColor(DPalette::WindowText, QColor("#526A7F"));
//            m_logHits->setPalette(pa);
        } else if (themeType == DGuiApplicationHelper::DarkType)
        {
            pa = palette();
            pa.setColor(DPalette::Background, QColor("#292929"));
            setPalette(pa);
//            pa = m_title->palette();
//            pa.setColor(DPalette::WindowText, QColor("#C0C6D4"));
//            m_title->setPalette(pa);
//            pa = m_hitsTitle->palette();
//            pa.setColor(DPalette::WindowText, QColor("#C0C6D4"));
//            m_hitsTitle->setPalette(pa);
//            pa = m_logHits->palette();
//            pa.setColor(DPalette::WindowText, QColor("#6D7C88"));
//            m_logHits->setPalette(pa);
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
        if (DTK_NAMESPACE::DCORE_NAMESPACE::DSysInfo::isCommunityEdition() == true) {
            m_logHits->setText(tr("The error log will be uploaded automatically with the feedback. We cannot improve without your feedback"));
            m_rebootLater->setText(tr("Submit Feedback"));
            m_logHits->adjustSize();
            m_rebootLater->disconnect();
            connect(m_rebootLater, &DPushButton::clicked,
            this, [ = ]() {
                // FIXME: call feedback 社区版保持链接进社区
                QProcess::startDetached("deepin-feedback");
            });
            break;
        } else {
            m_rebootLater->setText(tr("After-Sale Services"));
            m_logHits->adjustSize();
            m_rebootLater->disconnect();
            connect(m_rebootLater, &DPushButton::clicked,
            this, [ = ]() {
                // FIXME: call service-support  fix bug 19711 专业版不再调用deepin-feedback链接进社区，而是调用服务与支持客户端
                QDBusInterface syssupport("com.deepin.dde.ServiceAndSupport", "/com/deepin/dde/ServiceAndSupport", "com.deepin.dde.ServiceAndSupport");
                syssupport.call("showWindow");
            });
            break;
        }
    case BMHandler::USBFormatError:
    case BMHandler::USBSizeError:
    case BMHandler::USBMountFailed:
    case BMHandler::ExtractImgeFailed:
        m_logHits->setText(BMHandler::errorString(errorType));
        m_rebootLater->setText(tr("Close"));
        m_rebootLater->disconnect();
        connect(m_rebootLater, &DPushButton::clicked,
        this, [ = ]() {
            qApp->exit(0);
        });
        break;
    }
    m_hitsTitle->setText(tr("Sorry, process failed"));
    m_title->setText(tr("Process failed"));
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
