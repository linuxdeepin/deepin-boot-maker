#include "dwindow.h"
#include <QCursor>
#include <QGuiApplication>

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
    QObject::connect(qApp, SIGNAL(focusWindowChanged(QWindow*)), this, SLOT(focusChanged(QWindow *)));

}

DWindow::~DWindow()
{
}

QPoint DWindow::getCursorPos()
{
    return QCursor::pos();
}

void DWindow::focusChanged(QWindow *win)
{
    Q_EMIT windowFocusChanged(win);
}

void DWindow::mousePressEvent(QMouseEvent *ev){
    //qDebug() << "Event:" << ev->x() << "," << ev->y();
    QPointF p = QPointF(ev->x(), ev->y());
    DWindow::mousePressed(p);
    QQuickWindow::mousePressEvent(ev);
}
