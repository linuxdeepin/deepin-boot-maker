// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "dropframe.h"

#include <DApplicationHelper>

#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QDebug>
#include <QPainter>

static bool checkMimeData(const QMimeData *mimeData)
{
    if (!mimeData->hasUrls()) {
        return false;
    }

    QList<QUrl> urlList = mimeData->urls();
    if (1 != urlList.size()) {
        return false;
    }

    QFileInfo info(urlList.first().toLocalFile());
    if ("iso" != info.suffix().toLower()) {
        return false;
    }

    return true;
}

DropFrame::DropFrame(DWidget *parent) : DFrame(parent)
{
    setAcceptDrops(true);
    setFrameStyle(QFrame::NoFrame);
}

void DropFrame::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (!checkMimeData(mimeData)) {
        return;
    }

    event->acceptProposedAction();
    emit fileAboutAccept();
}

void DropFrame::dragLeaveEvent(QDragLeaveEvent *event)
{
    DFrame::dragLeaveEvent(event);
    emit fileCancel();
}

void DropFrame::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (!checkMimeData(mimeData)) {
        return;
    }

    emit fileDrop(mimeData->urls().first().toLocalFile());
    emit fileCancel();
}

void DropFrame::paintEvent(QPaintEvent *e)
{
    if (this->property("active").toBool()) {
        QPixmap pixmap;
        DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
        if (themeType == DGuiApplicationHelper::LightType) {
            pixmap = QPixmap(":/theme/light/image/dash.svg").scaled(this->size());
        } else if (themeType == DGuiApplicationHelper::DarkType) {
            pixmap = QPixmap(":/theme/dark/image/dash.svg").scaled(this->size());
        }
        QPainter painter(this);
        painter.drawPixmap(this->rect(), pixmap);
    }
    DFrame::paintEvent(e);
}
