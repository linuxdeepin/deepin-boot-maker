#include "isoselectview.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>

#include "suggestbutton.h"
#include "widgetutil.h"

DWIDGET_USE_NAMESPACE

const QString s_linkTemplate = "<a href='%1' style='text-decoration: none; color: #0066ec;'>%2</a>";

ISOSelectView::ISOSelectView(QWidget *parent) : QFrame(parent)
{
    setObjectName("ISOSelectView");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 9, 0, 0);

    QLabel *m_title = new QLabel(tr("请选择光盘文件"));
    m_title->setFixedHeight(38);
    m_title->setStyleSheet("font-size: 26px;");

    QLabel *isoIcon = new QLabel;
    isoIcon->setPixmap(QPixmap(":/theme/light/image/media-optical-96px.png"));

    QFrame *isoPanel = new QFrame;
    isoPanel->setObjectName("IosPanel");
    isoPanel->setFixedSize(410, 320);

    QVBoxLayout *isoPanelLayout = new QVBoxLayout(isoPanel);
    isoPanelLayout->setMargin(0);

    m_fileLabel = new QLabel(tr("拖拽镜像文件到此"));
    m_fileLabel->setObjectName("IsoFileName");
    m_fileLabel->setFixedHeight(15);

    m_hits = new QLabel(tr("Or"));
    m_hits->setObjectName("IsoHits");
    m_hits->setFixedHeight(15);

    QWidget *spliter = new QWidget;
    spliter->setObjectName("IsoSpliter");
    spliter->setFixedSize(220, 1);

    m_fileSelectButton = new QLabel();
    m_fileSelectButton->setObjectName("IsoFileSelect");
    m_fileSelectButton->setFixedHeight(15);
    m_fileSelectButton->setOpenExternalLinks(false);
    QString selectText = tr("Select File");
    QString linkText = QString(s_linkTemplate).arg(selectText).arg(selectText);
    m_fileSelectButton->setText(linkText);

    isoPanelLayout->addSpacing(62);
    isoPanelLayout->addWidget(isoIcon, 0, Qt::AlignCenter);
    isoPanelLayout->addSpacing(5);
    isoPanelLayout->addWidget(m_fileLabel, 0, Qt::AlignCenter);
    isoPanelLayout->addSpacing(7);
    isoPanelLayout->addWidget(m_hits, 0, Qt::AlignCenter);
    isoPanelLayout->addSpacing(10);
    isoPanelLayout->addWidget(spliter, 0, Qt::AlignCenter);
    isoPanelLayout->addSpacing(12);
    isoPanelLayout->addWidget(m_fileSelectButton, 0, Qt::AlignCenter);
    isoPanelLayout->addStretch();

    SuggestButton *netSetp = new SuggestButton();
    netSetp->setObjectName("NextStepButton");
    netSetp->setText(tr("Next Setup"));

    mainLayout->addWidget(m_title, 0, Qt::AlignCenter);
    mainLayout->addSpacing(24);
    mainLayout->addWidget(isoPanel, 0, Qt::AlignCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(netSetp, 0, Qt::AlignCenter);

    this->setStyleSheet(WidgetUtil::getQss(":/theme/light/ISOSelectView.theme"));

    connect(m_fileSelectButton, &QLabel::linkActivated, this, [=](const QString& link){
        qDebug()<< link;
    });
        onFileSelected("aaaaa");
}

void ISOSelectView::onFileSelected(const QString &file)
{
    m_fileLabel->setText(file);
    m_hits->setText("error");
    QString selectText = tr("Re Select File");
    QString linkText = QString(s_linkTemplate).arg(selectText).arg(selectText);
    m_fileSelectButton->setText(linkText);
}
