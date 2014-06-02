#include "dwindow.h"
#include <QCursor>

DOverrideWindow::DOverrideWindow(DWindow *parent)
    :DWindow(parent)
{
    QSurfaceFormat sformat;
    sformat.setAlphaBufferSize(8);
    this->setFormat(sformat);
    this->setClearBeforeRendering(true);

    this->setFlags(Qt::Tool|Qt::FramelessWindowHint);
}

DOverrideWindow::~DOverrideWindow()
{
}

DWindow::DWindow(QQuickWindow *parent)
    :QQuickWindow(parent)
{
    QSurfaceFormat sformat;
    sformat.setAlphaBufferSize(8);
    this->setFormat(sformat);
    this->setClearBeforeRendering(true);
}

DWindow::~DWindow()
{
}

QPoint DWindow::getCursorPos()
{
    return QCursor::pos();
}

void DOverrideWindow::mousePressEvent(QMouseEvent *ev){
    //qDebug() << "Event:" << ev->x() << "," << ev->y();
    QPointF p = QPointF(ev->x(), ev->y());
    DOverrideWindow::mousePressed(p);
    QQuickWindow::mousePressEvent(ev);
}
