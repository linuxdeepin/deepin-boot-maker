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

    m_title = new QLabel(tr("Make Successfully"));
    m_title->setFixedHeight(38);
    m_title->setStyleSheet("font-size: 26px;");

    m_resultIcon = new QLabel();
    m_resultIcon->setObjectName("ResultIcon");
    m_resultIcon->setPixmap(QPixmap(":/theme/light/image/success.png"));

    m_hitsTitle = new QLabel(tr("Make successfully, reboot to experience"));
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
    m_rebootLater->setText(tr("Reboot later"));
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
    mainLayout->addSpacing(15);
    mainLayout->addWidget(m_rebootNow, 0, Qt::AlignCenter);

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
        m_logHits->setText(tr("The error log will be uploaded automatically with feedback. We can not improve without your feedback and support"));
        m_rebootLater->setText(tr("Feedback"));
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
    m_resultIcon->setPixmap(QPixmap(":/theme/light/image/fail.png"));
    m_logHits->show();
    m_rebootNow->hide();
}

void ResultView::onLogLinkActivated(const QString &link)
{
    if (link == "#show_log") {
        QDesktopServices::openUrl(QUrl::fromLocalFile(Dtk::Util::DLogManager::getlogFilePath()));
    }
}
