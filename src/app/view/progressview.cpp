#include "progressview.h"

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

#include <bminterface.h>

ProgressView::ProgressView(QWidget *parent) : QWidget(parent)
{
    setObjectName("ProgressView");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 9, 0, 0);

    QLabel *m_title = new QLabel(tr("Making"));
    m_title->setFixedHeight(38);
    m_title->setStyleSheet("font-size: 26px;");

    auto waterProgress = new DWaterProgress;

    QLabel *m_hitsTitle = new QLabel(tr("Making the disk, please wait..."));
    m_hitsTitle->setObjectName("ProgressHitsTitle");

    QLabel *m_hits = new QLabel(tr("Please do not remove the disk or shut down the computer when making"));
    m_hits->setObjectName("ProgressHits");
    m_hits->setFixedSize(390, 60);
    m_hits->setWordWrap(true);
    m_hits->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    SuggestButton *start = new SuggestButton();
    start->setObjectName("ProgressCancel");
    start->setText(tr("Cancel"));

    mainLayout->addWidget(m_title, 0, Qt::AlignCenter);
    mainLayout->addSpacing(91);
    mainLayout->addWidget(waterProgress, 0, Qt::AlignCenter);
    mainLayout->addSpacing(28);
    mainLayout->addWidget(m_hitsTitle, 0, Qt::AlignCenter);
    mainLayout->addSpacing(4);
    mainLayout->addWidget(m_hits, 0, Qt::AlignCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(start, 0, Qt::AlignCenter);

    waterProgress->setProgress(0);
    waterProgress->start();

    this->setStyleSheet(WidgetUtil::getQss(":/theme/light/ProgressView.theme"));

//    connect(start, &SuggestButton::clicked, this, &ProgressView::testCancel);
    start->hide();
    connect(BMInterface::instance(), &BMInterface::reportProgress,
    this, [ = ](quint32 current, quint32 error, const QString & title, const QString & description) {
        qDebug() << error << current << title << description;
        waterProgress->setProgress(static_cast<int>(current));
        if (current >= 100) {
            emit finish(error, title, description);
        }
    });
}
