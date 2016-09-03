#include "resultview.h"

#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QListWidget>

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

    QLabel *m_title = new QLabel(tr("制作成功"));
    m_title->setFixedHeight(38);
    m_title->setStyleSheet("font-size: 26px;");

    QLabel *m_resultIcon = new QLabel();
    m_resultIcon->setObjectName("ResultIcon");
    m_resultIcon->setPixmap(QPixmap(":/theme/light/image/success.png"));

    QLabel *m_hitsTitle = new QLabel(tr("制作成功，请重启电脑"));
    m_hitsTitle->setObjectName("ResulteHitsTitle");

    SuggestButton *rebootLater = new SuggestButton();
    rebootLater->setObjectName("RebootLater");
    rebootLater->setText(tr("稍后重启"));

    SuggestButton *rebootNow = new SuggestButton();
    rebootNow->setObjectName("RebootLater");
    rebootNow->setText(tr("立即重启"));

    mainLayout->addWidget(m_title, 0, Qt::AlignCenter);
    mainLayout->addSpacing(85);
    mainLayout->addWidget(m_resultIcon, 0, Qt::AlignCenter);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(m_hitsTitle, 0, Qt::AlignCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(rebootLater, 0, Qt::AlignCenter);
    mainLayout->addSpacing(15);
    mainLayout->addWidget(rebootNow, 0, Qt::AlignCenter);

    this->setStyleSheet(WidgetUtil::getQss(":/theme/light/ResultView.theme"));
}
