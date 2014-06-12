#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include "ddroparea.h"
#include <QDebug>
#include <QFileInfo>

DDropArea::DDropArea(QQuickItem *parent)
        : QQuickItem(parent)
{
    setFlag(ItemAcceptsDrops, true);
}

void DDropArea::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}
#include <QMimeDatabase>

void DDropArea::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();

    if (urls.isEmpty())
       return;
    QString fileName = urls.first().toLocalFile();

    if (fileName.isEmpty())
        return;

    QMimeDatabase db;
    QMimeType type = db.mimeTypeForFile(fileName);
    qDebug() << "Mime type:" << type.name();

    if ("application/x-cd-image" != type.name()) {
        return;
    }

    emit textDrop(fileName);
}
