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
#include "widgetutil.h"
#include "bminterface.h"

#include <DLabel>
#include <DFileDialog>
#include <DApplicationHelper>
#include <DUtil>

#include <QStyle>
#include <QVBoxLayout>
#include <QStandardPaths>
#include <QDebug>
#include <QFontDatabase>
#include <QtConcurrent>

DWIDGET_USE_NAMESPACE

const QString s_linkTemplatelight = "<a href='%1' style='text-decoration: none; color: #0066EC;'>%2</a>";
const QString s_linkTemplatedark = "<a href='%1' style='text-decoration: none; color: #0082FA;'>%2</a>";
const QString s_stateTemplate = "<a style='text-decoration: none; color: #FF5A5A;'>%1</a>";

ThreadCheckFile::ThreadCheckFile()
{
    restart = false;
    m_file = "";
}

void ThreadCheckFile::setFile(QString file)
{
    m_file = file;
}

void ThreadCheckFile::setRestart()
{
    restart = true;
}

void ThreadCheckFile::run()
{
    restart = true;
    bool checkok = false;
    while (restart) {
        restart = false;
        checkok = BMInterface::instance()->checkfile(m_file);
    }
    emit checkFileFinish(checkok);
}

ISOSelectView::ISOSelectView(DWidget *parent) : DWidget(parent)
{
    setObjectName("ISOSelectView");
    setAutoFillBackground(true);
//    setFrameStyle(QFrame::NoFrame);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 1, 0, 0);
    mainLayout->setSpacing(0);

    int lcdFontId = QFontDatabase::addApplicationFont(":/theme/SourceHanSansSC-Medium.ttf");
    QStringList m_fontList;
    m_fontList.clear();
    if (lcdFontId != -1) {
        m_fontList << QFontDatabase::applicationFontFamilies(lcdFontId);
    }

    m_title = new DLabel(tr("Select an ISO image file"));
    m_title->setFixedHeight(35);
    QFont qf = m_title->font();
    if (m_fontList.size() > 0)
        qf.setFamily(m_fontList.at(0));
    qf.setPixelSize(24);
    m_title->setFont(qf);

    DLabel *isoIcon = new DLabel(this);
    isoIcon->setObjectName("ISOIcon");
    isoIcon->setFixedSize(96, 96);
    isoIcon->setPixmap(WidgetUtil::getDpiPixmap(":/theme/light/image/media-optical-96px.svg", this));

    growIcon = new DLabel(this);
    growIcon->setObjectName("GrowIcon");
//    growIcon->setPixmap(WidgetUtil::getDpiPixmap(":/theme/light/image/glow.svg", this));
    growIcon->setFixedSize(220, 220);
    growIcon->hide();

    isoPanel = new DropFrame;
    isoPanel->setObjectName("IosPanel");
    isoPanel->setFixedSize(410, 300);

    QVBoxLayout *isoPanelLayout = new QVBoxLayout(isoPanel);
    isoPanelLayout->setMargin(0);

#ifdef Q_OS_WIN
    m_fileLabel = new DLabel("");
#else
    m_fileLabel = new DLabel(tr("Drag an ISO image file and drop it here"));
#endif
    m_fileLabel->setObjectName("IsoFileName");
    m_fileLabel->setFixedHeight(15);
    qf = m_fileLabel->font();
    qf.setPixelSize(12);
    m_fileLabel->setFont(qf);
//    m_fileLabel->setFixedHeight(18);

//    m_stateLabel = new QLabel();
//    qf = m_stateLabel->font();
//    qf.setPixelSize(12);
//    m_stateLabel->setFont(qf);
//    m_stateLabel->setFixedHeight(38);
//    m_stateLabel->hide();

#ifdef Q_OS_WIN
    m_hits = new DLabel("");
#else
    m_hits = new DLabel(tr("OR"));
#endif
    m_hits->setObjectName("IsoHits");
    m_hits->setFixedHeight(20);
    qf = m_hits->font();
    qf.setPixelSize(12);
    m_hits->setFont(qf);

    spliter = new DLabel;
    spliter->setObjectName("IsoSpliter");
    spliter->setFixedSize(230, 1);
    spliter->setAutoFillBackground(true);
//    spliter->setPixmap(WidgetUtil::getDpiPixmap(":/theme/light/image/dash_line.svg", this));

    m_fileSelect = new DLabel();
    m_fileSelect->setObjectName("IsoFileSelect");
//    m_fileSelect->setFixedHeight(15);
//    m_fileSelect->setOpenExternalLinks(false);
    m_selectText = tr("Select an ISO image file");
    m_fileSelect->setFixedHeight(18);
//    QString linkText = QString(s_linkTemplatelight).arg(m_selectText).arg(m_selectText);
//    DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
//    if (themeType == DGuiApplicationHelper::DarkType) {
//        linkText = QString(s_linkTemplatedark).arg(m_selectText).arg(m_selectText);
//    }
//    m_fileSelect->setText(linkText);
    qf = m_fileSelect->font();
//    if (m_fontList.size() > 1)
//        qf.setFamily(m_fontList.at(1));
    qf.setPixelSize(12);
    qf.setBold(false);
    m_fileSelect->setFont(qf);

    isoPanelLayout->addSpacing(63);
    isoPanelLayout->addWidget(isoIcon, 0, Qt::AlignHCenter);
    isoPanelLayout->addSpacing(5);
    isoPanelLayout->addWidget(m_fileLabel, 0, Qt::AlignHCenter);
//    isoPanelLayout->addSpacing(4);
//    isoPanelLayout->addWidget(m_stateLabel, 0, Qt::AlignCenter);
    isoPanelLayout->addSpacing(0);
    isoPanelLayout->addWidget(m_hits, 0, Qt::AlignHCenter);
    isoPanelLayout->addSpacing(15);
    isoPanelLayout->addWidget(spliter, 0, Qt::AlignHCenter);
    isoPanelLayout->addSpacing(10);
    isoPanelLayout->addWidget(m_fileSelect, 0, Qt::AlignHCenter);
    isoPanelLayout->addStretch();



    m_checkFile = new DLabel();
    m_checkFile->setObjectName("IsoFileSelect");
    m_checkFile->setFixedHeight(18);
    qf = m_checkFile->font();
    m_checkFile->setText("");
    qf.setPixelSize(12);
    m_checkFile->setFont(qf);

//    m_nextSetp = new SuggestButton();
    m_nextSetp = new DPushButton();
    m_nextSetp->setFocusPolicy(Qt::NoFocus);
    m_nextSetp->setFixedSize(310, 36);
    m_nextSetp->setObjectName("NextStepButton");
    m_nextSetp->setText(tr("Next"));
    qf = m_nextSetp->font();
    if (m_fontList.size() > 0)
        qf.setFamily(m_fontList.at(0));
    qf.setPixelSize(14);
    m_nextSetp->setFont(qf);
    m_nextSetp->setDisabled(true);

    mainLayout->addWidget(m_title, 0, Qt::AlignCenter);
    mainLayout->addSpacing(32);
    mainLayout->addWidget(isoPanel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(16);
    mainLayout->addWidget(m_checkFile, 0, Qt::AlignCenter);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(m_nextSetp, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    slot_ThemeChange();
    connect(&t_checkfile, &ThreadCheckFile::checkFileFinish,
    this, [ = ](bool result) {
        m_checkFile->setText("");
        QString stateText = "";
        if (!result)
            stateText = tr("Illegal ISO image file");
        m_nextSetp->setDisabled(false);
        if ("" != stateText) {
            QString stateTemplateText = QString(s_stateTemplate).arg(stateText);
            m_hits->setText(stateTemplateText);
            m_nextSetp->setDisabled(true);
            m_isoFilePath = "";
        }
    });
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            this, &ISOSelectView :: slot_ThemeChange);

#ifdef Q_OS_WIN
//    m_fileLabel->hide();
    spliter->hide();
//    m_hits->hide();
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
        isoPanel->update();
//        this->style()->unpolish(isoPanel);
//        this->style()->polish(isoPanel);
    });
    connect(isoPanel, &DropFrame::fileCancel, this, [ = ]() {
        growIcon->hide();
        isoPanel->setProperty("active", false);
        isoPanel->update();
//        this->style()->unpolish(isoPanel);
//        this->style()->polish(isoPanel);
    });

    connect(m_fileSelect, &DLabel::linkActivated, this, [ = ](const QString & /*link*/) {
        DFileDialog fileDlg(this);
        fileDlg.setViewMode(DFileDialog::Detail);
        fileDlg.setFileMode(DFileDialog::ExistingFile);
        fileDlg.setDirectory(QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first());
        fileDlg.setNameFilter("ISO (*.iso)");
        fileDlg.selectNameFilter("ISO (*.iso)");
        if (DFileDialog::Accepted == fileDlg.exec()) {
            QString text = fileDlg.selectedFiles().first();
            onFileSelected(text);
        }
    });

//    connect(m_nextSetp, &SuggestButton::clicked, this, &ISOSelectView::isoFileSelected);
    connect(m_nextSetp, &DPushButton::clicked, this, &ISOSelectView::isoFileSelected);
    connect(isoPanel, &DropFrame::fileDrop, this, &ISOSelectView::onFileSelected);

}

//void ISOSelectView :: checkFileResult(bool result)
//{
//    m_checkFile->setText("");
//    QString stateText = "";
//    if (!result) {
//        stateText = tr("Illegal ISO image file");
//    }
//    m_nextSetp->setDisabled(false);
//    if ("" != stateText) {
//        QString stateTemplateText = QString(s_stateTemplate).arg(stateText);
//        m_hits->setText(stateTemplateText);
//        m_nextSetp->setDisabled(true);
//        m_isoFilePath = "";
//    }
//}

void ISOSelectView :: slot_ThemeChange()
{
    DPalette pa;
    DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
    if (themeType == DGuiApplicationHelper::LightType) {
        pa = palette();
//      pa.setColor(DPalette::Background, QColor("#FFFFFF"));
        pa.setColor(DPalette::Background, QColor(255, 255, 255));
        setPalette(pa);
        pa = m_title->palette();
        pa.setColor(DPalette::WindowText, QColor("#001A2E"));
        m_title->setPalette(pa);
        pa = isoPanel->palette();
        pa.setColor(DPalette::Background, QColor(255, 255, 255, 13));
        isoPanel->setPalette(pa);
        pa = m_fileLabel->palette();
        pa.setColor(DPalette::Text, QColor("#B1B1B1"));
        m_fileLabel->setPalette(pa);
        pa = m_hits->palette();
        pa.setColor(DPalette::WindowText, QColor("#424242"));
        m_hits->setPalette(pa);
        growIcon->setPixmap(WidgetUtil::getDpiPixmap(":/theme/light/image/disk_big.svg", this));
        spliter->setPixmap(WidgetUtil::getDpiPixmap(":/theme/light/image/dash_line.svg", this));
        QString linkText = QString(s_linkTemplatelight).arg(m_selectText).arg(m_selectText);
        m_fileSelect->setText(linkText);
        pa = m_checkFile->palette();
        pa.setColor(DPalette::WindowText, QColor("#414D68"));
        m_checkFile->setPalette(pa);
    } else if (themeType == DGuiApplicationHelper::DarkType) {
        pa = palette();
        pa.setColor(DPalette::Background, QColor("#292929"));
        setPalette(pa);
        pa = m_title->palette();
        pa.setColor(DPalette::WindowText, QColor("#C0C6D4"));
        m_title->setPalette(pa);
        pa = isoPanel->palette();
        pa.setColor(DPalette::Background, QColor(0, 0, 0, 13));
        isoPanel->setPalette(pa);
        pa = m_fileLabel->palette();
        pa.setColor(DPalette::Text, QColor("#6D7C88"));
        m_fileLabel->setPalette(pa);
        pa = m_hits->palette();
        pa.setColor(DPalette::WindowText, QColor("#E3E3E3"));
        m_hits->setPalette(pa);
        growIcon->setPixmap(WidgetUtil::getDpiPixmap(":/theme/dark/image/disc_bg.png", this));
        spliter->setPixmap(WidgetUtil::getDpiPixmap(":/theme/dark/image/dash_line_dark.svg", this));
        QString linkText = QString(s_linkTemplatedark).arg(m_selectText).arg(m_selectText);
        m_fileSelect->setText(linkText);
        pa = m_checkFile->palette();
        pa.setColor(DPalette::WindowText, QColor("#6D7C88"));
        m_checkFile->setPalette(pa);
    }
}

void ISOSelectView::onFileSelected(const QString &file)
{
//    bool checkok = false;
//    QEventLoop loop;
//    connect(this, SIGNAL(checkFileFinish()), &loop, SLOT(quit()));
    QFileInfo info(file);
    m_fileLabel->setText(info.fileName());
    m_fileLabel->show();
    m_hits->setText("");
    m_selectText = tr("Reselect an ISO image file");
    m_checkFile->setText(tr("Detecting ISO file, please wait..."));
    slot_ThemeChange();
    m_nextSetp->setDisabled(true);
    m_isoFilePath = file;
//    BMInterface::instance()->checkfile(file);

    t_checkfile.setFile(file);
    if (t_checkfile.isRunning()) {
        t_checkfile.setRestart();
    } else {
        t_checkfile.start();
    }
}

//void ISOSelectView::onFileSelected(const QString &file)
//{
//    bool checkok = false;
//    checkok = BMInterface::instance()->checkfile(file);
//    QFileInfo info(file);
//    m_fileLabel->setText(info.fileName());
//    m_fileLabel->show();
//    m_hits->setText("");
//    m_selectText = tr("Reselect an ISO image file");
//    m_checkFile->setText(tr("Detecting ISO file, please wait..."));
//    slot_ThemeChange();
//    m_nextSetp->setDisabled(true);
////    m_checkFile->setText("");
//    QString stateText = "";
//    if (!checkok)
//        stateText = tr("Illegal ISO image file");
////    QString linkText = QString(s_linkTemplate).arg(selectText).arg(selectText);
////    m_fileSelect->setText(linkText);
//    m_nextSetp->setDisabled(false);
////    m_stateLabel->hide();
//    if ("" != stateText) {
//        QString stateTemplateText = QString(s_stateTemplate).arg(stateText);
////        m_stateLabel->setText(stateTemplateText);
////        m_stateLabel->show();
//        m_hits->setText(stateTemplateText);
//        m_nextSetp->setDisabled(true);
//    }
//    m_isoFilePath = file;
//    qDebug() << "onFileSelected file:" << file;
//}


void ISOSelectView::onFileVerfiyFinished(bool ok)
{
    if (ok) {
    }
}
