#include "dropframe.h"

#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QDebug>

static bool checkMimeData(const QMimeData *mimeData)
{
    if (!mimeData->hasUrls()) { return false; }

    QList<QUrl> urlList = mimeData->urls();
    if (1 != urlList.size()) { return false;}

    QFileInfo info(urlList.first().toLocalFile());
    if ("iso" != info.suffix().toLower()) { return false; }

    return true;
}

DropFrame::DropFrame(QWidget *parent) : QFrame(parent)
{
    setAcceptDrops(true);
}

void DropFrame::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (!checkMimeData(mimeData)) { return; }

    event->acceptProposedAction();
}

void DropFrame::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (!checkMimeData(mimeData)) { return; }

    emit fileDrop(mimeData->urls().first().toLocalFile());
}
