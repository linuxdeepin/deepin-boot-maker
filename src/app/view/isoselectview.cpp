#include "isoselectview.h"

#include <QStyle>
#include <QLabel>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>

#include "suggestbutton.h"
#include "widgetutil.h"
#include "dropframe.h"

#include <DUtil>
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

    QLabel *isoIcon = new QLabel(this);
    isoIcon->setObjectName("ISOIcon");
    isoIcon->setPixmap(QPixmap(":/theme/light/image/media-optical-96px.png"));
    isoIcon->setFixedSize(96, 96);

    QLabel *growIcon = new QLabel(this);
    growIcon->setObjectName("GrowIcon");
    growIcon->setPixmap(QPixmap(":/theme/light/image/glow.png"));
    growIcon->setFixedSize(220, 220);
    growIcon->hide();

    DropFrame *isoPanel = new DropFrame;
    isoPanel->setObjectName("IosPanel");
    isoPanel->setFixedSize(412, 322);

    QVBoxLayout *isoPanelLayout = new QVBoxLayout(isoPanel);
    isoPanelLayout->setMargin(0);

    m_fileLabel = new QLabel(tr("Drop ISO Here"));
    m_fileLabel->setObjectName("IsoFileName");
    m_fileLabel->setFixedHeight(18);

    m_hits = new QLabel(tr("Or"));
    m_hits->setObjectName("IsoHits");
    m_hits->setFixedHeight(18);

    QLabel *spliter = new QLabel;
    spliter->setObjectName("IsoSpliter");
    spliter->setFixedSize(230, 1);

    m_fileSelect = new QLabel();
    m_fileSelect->setObjectName("IsoFileSelect");
    m_fileSelect->setFixedHeight(15);
    m_fileSelect->setOpenExternalLinks(false);
    QString selectText = tr("Select File");
    QString linkText = QString(s_linkTemplate).arg(selectText).arg(selectText);
    m_fileSelect->setText(linkText);

    isoPanelLayout->addSpacing(62);
    isoPanelLayout->addWidget(isoIcon, 0, Qt::AlignCenter);
    isoPanelLayout->addSpacing(5);
    isoPanelLayout->addWidget(m_fileLabel, 0, Qt::AlignCenter);
    isoPanelLayout->addSpacing(4);
    isoPanelLayout->addWidget(m_hits, 0, Qt::AlignCenter);
    isoPanelLayout->addSpacing(7);
    isoPanelLayout->addWidget(spliter, 0, Qt::AlignCenter);
    isoPanelLayout->addSpacing(12);
    isoPanelLayout->addWidget(m_fileSelect, 0, Qt::AlignCenter);
    isoPanelLayout->addStretch();

    m_nextSetp = new SuggestButton();
    m_nextSetp->setObjectName("NextStepButton");
    m_nextSetp->setText(tr("Next Setup"));
    m_nextSetp->setDisabled(true);

    mainLayout->addWidget(m_title, 0, Qt::AlignCenter);
    mainLayout->addSpacing(24);
    mainLayout->addWidget(isoPanel, 0, Qt::AlignCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(m_nextSetp, 0, Qt::AlignCenter);

    this->setStyleSheet(WidgetUtil::getQss(":/theme/light/ISOSelectView.theme"));

    connect(isoPanel, &DropFrame::fileAboutAccept, this, [ = ]() {
        growIcon->show();
        auto center = isoIcon->geometry().center();
        growIcon->move(center);
        auto topleft = growIcon->mapFromGlobal(isoIcon->mapToGlobal(center));
        auto offset = 220 - 96;
        topleft.setX(topleft.x() - offset / 2);
        topleft.setY(topleft.y() - offset / 2);
        growIcon->move(topleft);
        isoPanel->setProperty("active", true);
        this->style()->unpolish(isoPanel);
        this->style()->polish(isoPanel);
    });
    connect(isoPanel, &DropFrame::fileCancel, this, [ = ]() {
        growIcon->hide();
        isoPanel->setProperty("active", false);
        this->style()->unpolish(isoPanel);
        this->style()->polish(isoPanel);
    });

    connect(m_fileSelect, &QLabel::linkActivated, this, [ = ](const QString & /*link*/) {
        QFileDialog fileDlg(this);
        fileDlg.setViewMode(QFileDialog::Detail);
        fileDlg.setFileMode(QFileDialog::ExistingFile);
        fileDlg.setDirectory(QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first());
        fileDlg.setNameFilter("ISO (*.iso);;");
        if (QFileDialog::Accepted == fileDlg.exec()) {
            QString text = fileDlg.selectedFiles().first();
            onFileSelected(text);
        }
    });

    connect(m_nextSetp, &SuggestButton::clicked, this, &ISOSelectView::isoFileSelected);
    connect(isoPanel, &DropFrame::fileDrop, this, &ISOSelectView::onFileSelected);

}

void ISOSelectView::onFileSelected(const QString &file)
{
    QFileInfo info(file);
    m_fileLabel->setText(info.fileName());
    m_hits->setText("");
    QString selectText = tr("重新选择文件");
    QString linkText = QString(s_linkTemplate).arg(selectText).arg(selectText);
    m_fileSelect->setText(linkText);
    m_nextSetp->setDisabled(false);
    m_isoFilePath = file;
}

void ISOSelectView::onFileVerfiyFinished(bool ok)
{
    if (ok) {
    }
}
