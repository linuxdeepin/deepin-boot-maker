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

    QLabel *m_title = new QLabel(tr("Select an ISO image file"));
    m_title->setFixedHeight(38);
    m_title->setStyleSheet("font-size: 26px;");

    QLabel *isoIcon = new QLabel(this);
    isoIcon->setObjectName("ISOIcon");
    isoIcon->setFixedSize(96, 96);
    isoIcon->setPixmap(WidgetUtil::getDpiPixmap(":/theme/light/image/media-optical-96px.svg", this));

    QLabel *growIcon = new QLabel(this);
    growIcon->setObjectName("GrowIcon");
    growIcon->setPixmap(WidgetUtil::getDpiPixmap(":/theme/light/image/glow.svg", this));
    growIcon->setFixedSize(220, 220);
    growIcon->hide();

    DropFrame *isoPanel = new DropFrame;
    isoPanel->setObjectName("IosPanel");
    isoPanel->setFixedSize(412, 322);

    QVBoxLayout *isoPanelLayout = new QVBoxLayout(isoPanel);
    isoPanelLayout->setMargin(0);

    m_fileLabel = new QLabel(tr("Drag an ISO image file and drop it here"));
    m_fileLabel->setObjectName("IsoFileName");
//    m_fileLabel->setFixedHeight(18);

    m_hits = new QLabel(tr("OR"));
    m_hits->setObjectName("IsoHits");
    m_hits->setFixedHeight(18);

    QLabel *spliter = new QLabel;
    spliter->setObjectName("IsoSpliter");
    spliter->setFixedSize(230, 1);

    m_fileSelect = new QLabel();
    m_fileSelect->setObjectName("IsoFileSelect");
//    m_fileSelect->setFixedHeight(15);
    m_fileSelect->setOpenExternalLinks(false);
    QString selectText = tr("Select an ISO image file");
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
    m_nextSetp->setText(tr("Next"));
    m_nextSetp->setDisabled(true);

    mainLayout->addWidget(m_title, 0, Qt::AlignCenter);
    mainLayout->addSpacing(24);
    mainLayout->addWidget(isoPanel, 0, Qt::AlignCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(m_nextSetp, 0, Qt::AlignCenter);

    this->setStyleSheet(WidgetUtil::getQss(":/theme/light/ISOSelectView.theme"));

#ifdef Q_OS_WIN
    m_fileLabel->hide();
    spliter->hide();
    m_hits->hide();
#endif
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
        fileDlg.setNameFilter("ISO (*.iso)");
        fileDlg.selectNameFilter("ISO (*.iso)");
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
    m_fileLabel->show();
    m_hits->setText("");
    QString selectText = tr("Reselect an ISO image file");
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
