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

#include <QDebug>
#include <QLabel>
#include <QUrl>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QProcess>
#include <QListWidget>
#include <QDesktopServices>
#include <QApplication>

#include <DLog>

#include "suggestbutton.h"
#include "widgetutil.h"
#include "deviceinfoitem.h"
#include "devicelistwidget.h"
#include "dwaterprogress.h"

#include <bminterface.h>
#include <backend/bmhandler.h>

ResultView::ResultView(QWidget *parent) : QWidget(parent)
{
    setObjectName("ResultView");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 9, 0, 0);
    mainLayout->setSpacing(0);

    m_title = new QLabel(tr("Successful"));
    m_title->setFixedHeight(38);
    m_title->setStyleSheet("font-size: 26px;");

    m_resultIcon = new QLabel();
    m_resultIcon->setObjectName("ResultIcon");
    m_resultIcon->setPixmap(WidgetUtil::getDpiPixmap(":/theme/light/image/success.svg", this));

    m_hitsTitle = new QLabel();
    m_hitsTitle->setObjectName("ResulteHitsTitle");
    m_hitsTitle->setFixedWidth(340);

//    QString hitsFormat = "<a style='color:#b4b4b4; font-size:11px'>%1</a>";
//    QString tagBegin = "<a href='#show_log'><span style='text-decoration: underline; color:#1B85ff;'>";
//    QString tagEnd = "</span></a>";
//    QString log = tr("Installation logs are stored in %1HERE%2, you can upload to forum to help us solve your problem.");
    m_logHits = new QLabel(/*hitsFormat.arg(log.arg(tagBegin).arg(tagEnd))*/);
    m_logHits->setObjectName("ResultErrorDescription");
    m_logHits->setWordWrap(true);
    m_logHits->setFixedWidth(340);
    m_logHits->setFixedHeight(100);
//    connect(m_logHits, &QLabel::linkActivated, this, &ResultView::onLogLinkActivated);
//    m_logHits->setOpenExternalLinks(false);
    m_logHits->hide();

    m_rebootLater = new SuggestButton();
    m_rebootLater->setObjectName("RebootLater");
    m_rebootLater->setText(tr("Done"));
    m_rebootLater->setProperty("normal", true);/*
    rebootLater->style()->unpolish(rebootLater);
    rebootLater->style()->polish(rebootLater);*/

    m_rebootNow = new SuggestButton();
    m_rebootNow->setObjectName("RebootLater");
    m_rebootNow->setText(tr("Reboot now"));

    mainLayout->addWidget(m_title, 0, Qt::AlignCenter);
    mainLayout->addSpacing(85);
    mainLayout->addWidget(m_resultIcon, 0, Qt::AlignCenter);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(m_hitsTitle, 0, Qt::AlignCenter);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(m_logHits, 0, Qt::AlignCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(m_rebootLater, 0, Qt::AlignCenter);
//    mainLayout->addSpacing(15);
//    m_rebootNow->hide();
//    mainLayout->addWidget(m_rebootNow, 0, Qt::AlignCenter);

    this->setStyleSheet(WidgetUtil::getQss(":/theme/light/ResultView.theme"));

    connect(m_rebootNow, &SuggestButton::clicked,
    this, [ = ]() {
        BMInterface::instance()->reboot();
    });
}

void ResultView::updateResult(quint32 error, const QString &/*title*/, const QString &/*description*/)
{
    auto errorType = static_cast<BMHandler::ErrorType>(error);

    switch (errorType) {
    case BMHandler::NoError:
        m_rebootLater->disconnect();
        connect(m_rebootLater, &SuggestButton::clicked,
        this, [ = ]() {
            qApp->exit(0);
        });
        return;
    case BMHandler::SyscExecFailed:
        m_logHits->setText(tr("The error log will be uploaded automatically with the feedback. We cannot improve without your feedback"));
        m_rebootLater->setText(tr("Submit Feedback"));
        m_logHits->adjustSize();
        m_rebootLater->disconnect();
        connect(m_rebootLater, &SuggestButton::clicked,
        this, [ = ]() {
            // FIXME: call feedback
            QProcess::startDetached("deepin-feedback");
        });
        break;
    case BMHandler::USBFormatError:
    case BMHandler::USBSizeError:
    case BMHandler::USBMountFailed:
    case BMHandler::ExtractImgeFailed:
        m_logHits->setText(BMHandler::errorString(errorType));
        m_rebootLater->setText(tr("Close"));
        m_rebootLater->disconnect();
        connect(m_rebootLater, &SuggestButton::clicked,
        this, [ = ]() {
            qApp->exit(0);
        });
        break;
    }
    m_hitsTitle->setText(tr("Sorry, making failed"));
    m_title->setText(tr("Making failed"));
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
