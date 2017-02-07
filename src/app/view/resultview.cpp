#include "resultview.h"

#include <QDebug>
#include <QLabel>
#include <QUrl>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QListWidget>
#include <QDesktopServices>
#include <QApplication>

#include <DLog>

#include "suggestbutton.h"
#include "widgetutil.h"
#include "deviceinfoitem.h"
#include "devicelistwidget.h"
#include "dwaterprogress.h"

ResultView::ResultView(QWidget *parent) : QWidget(parent)
{
    setObjectName("ResultView");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 9, 0, 0);
    mainLayout->setSpacing(0);

    m_title = new QLabel(tr("制作成功"));
    m_title->setFixedHeight(38);
    m_title->setStyleSheet("font-size: 26px;");

    m_resultIcon = new QLabel();
    m_resultIcon->setObjectName("ResultIcon");
    m_resultIcon->setPixmap(QPixmap(":/theme/light/image/success.png"));

    m_hitsTitle = new QLabel(tr("制作成功，请重启电脑"));
    m_hitsTitle->setObjectName("ResulteHitsTitle");
    m_hitsTitle->setFixedWidth(340);

    QString hitsFormat = "<a style='color:#b4b4b4; font-size:11px'>%1</a>";
    QString tagBegin = "<a href='#show_log'><span style='text-decoration: underline; color:#1B85ff;'>";
    QString tagEnd = "</span></a>";
    QString log = tr("Installation logs are stored in %1HERE%2, you can upload to forum to help us solve your problem.");
    m_logHits = new QLabel(hitsFormat.arg(log.arg(tagBegin).arg(tagEnd)));
    m_logHits->setFixedWidth(340);
    m_logHits->setWordWrap(true);
    connect(m_logHits, &QLabel::linkActivated, this, &ResultView::onLogLinkActivated);
    m_logHits->setOpenExternalLinks(false);
    m_logHits->hide();

    m_rebootLater = new SuggestButton();
    m_rebootLater->setObjectName("RebootLater");
    m_rebootLater->setText(tr("稍后重启"));
    m_rebootLater->setProperty("normal", true);/*
    rebootLater->style()->unpolish(rebootLater);
    rebootLater->style()->polish(rebootLater);*/

    m_rebootNow = new SuggestButton();
    m_rebootNow->setObjectName("RebootLater");
    m_rebootNow->setText(tr("立即重启"));

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

    connect(m_rebootLater, &SuggestButton::clicked,
            this,[=](){
        qApp->exit(0);
    });

    connect(m_rebootNow, &SuggestButton::clicked,
            this,[=](){
        // FIXME:
    });
}

void ResultView::updateResult(quint32 error, const QString &/*title*/, const QString &description)
{
    if (0 == error) {
        return;
    }
    m_hitsTitle->setText(description);
    m_resultIcon->setPixmap(QPixmap(":/theme/light/image/fail.png"));
    m_logHits->show();
    m_rebootNow->hide();
    m_rebootLater->setText(tr("Failed"));
    m_title->setText(tr("Make failed"));
}

void ResultView::onLogLinkActivated(const QString &link)
{
    if (link == "#show_log") {
        QDesktopServices::openUrl(QUrl::fromLocalFile(Dtk::Util::DLogManager::getlogFilePath()));
    }
}
