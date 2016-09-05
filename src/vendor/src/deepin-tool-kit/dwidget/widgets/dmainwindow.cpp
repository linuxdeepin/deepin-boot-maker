#include "dmainwindow.h"
#include "dplatformwindowhandle.h"
#include "dapplication.h"
#include "dtitlebar.h"

#include "private/dmainwindow_p.h"

#include <QWindow>

/// shadow
#define SHADOW_COLOR_NORMAL QColor(0, 0, 0, 255 * 0.35)
#define SHADOW_COLOR_ACTIVE QColor(0, 0, 0, 255 * 0.6)

DWIDGET_BEGIN_NAMESPACE

DMainWindowPrivate::DMainWindowPrivate(DMainWindow *qq)
    : DObjectPrivate(qq)
{
    if (DApplication::isDXcbPlatform()) {
        handle = new DPlatformWindowHandle(qq, qq);
        titlebar = new DTitlebar(qq);

        qq->setWindowFlags(Qt::FramelessWindowHint);
        qq->setMenuWidget(titlebar);
    }
}

void DMainWindowPrivate::init()
{
    D_Q(DMainWindow);

    if (handle) {
        q->connect(handle, &DPlatformWindowHandle::borderColorChanged, q, &DMainWindow::borderColorChanged);
        q->connect(handle, &DPlatformWindowHandle::borderWidthChanged, q, &DMainWindow::borderWidthChanged);
        q->connect(handle, &DPlatformWindowHandle::clipPathChanged, q, &DMainWindow::clipPathChanged);
        q->connect(handle, &DPlatformWindowHandle::frameMarginsChanged, q, &DMainWindow::frameMarginsChanged);
        q->connect(handle, &DPlatformWindowHandle::frameMaskChanged, q, &DMainWindow::frameMaskChanged);
        q->connect(handle, &DPlatformWindowHandle::shadowColorChanged, q, &DMainWindow::shadowColorChanged);
        q->connect(handle, &DPlatformWindowHandle::shadowOffsetChanged, q, &DMainWindow::shadowOffsetChanged);
        q->connect(handle, &DPlatformWindowHandle::shadowRadiusChanged, q, &DMainWindow::shadowRadiusChanged);
        q->connect(handle, &DPlatformWindowHandle::windowRadiusChanged, q, &DMainWindow::windowRadiusChanged);
        q->connect(handle, &DPlatformWindowHandle::translucentBackgroundChanged, q, &DMainWindow::translucentBackgroundChanged);
        q->connect(handle, &DPlatformWindowHandle::enableSystemMoveChanged, q, &DMainWindow::enableSystemMoveChanged);
        q->connect(handle, &DPlatformWindowHandle::enableSystemResizeChanged, q, &DMainWindow::enableSystemResizeChanged);

        q->connect(qApp, &QGuiApplication::focusWindowChanged, q, [q] {
            if (q->isActiveWindow()) {
                q->setShadowColor(SHADOW_COLOR_ACTIVE);
            } else {
                q->setShadowColor(SHADOW_COLOR_NORMAL);
            }
        });
    }
}

DMainWindow::DMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , DObject(*new DMainWindowPrivate(this))
{
    d_func()->init();
}

DTitlebar *DMainWindow::titleBar() const
{
    D_DC(DMainWindow);

    return d->titlebar;
}

bool DMainWindow::isDXcbWindow() const
{
    D_DC(DMainWindow);

    return d->handle;
}

int DMainWindow::windowRadius() const
{
    D_DC(DMainWindow);

    if (!d->handle)
        return 0;

    return d->handle->windowRadius();
}

int DMainWindow::borderWidth() const
{
    D_DC(DMainWindow);

    if (!d->handle)
        return 0;

    return d->handle->borderWidth();
}

QColor DMainWindow::borderColor() const
{
    D_DC(DMainWindow);

    if (!d->handle)
        return QColor();

    return d->handle->borderColor();
}

int DMainWindow::shadowRadius() const
{
    D_DC(DMainWindow);

    if (!d->handle)
        return 0;

    return d->handle->shadowRadius();
}

QPoint DMainWindow::shadowOffset() const
{
    D_DC(DMainWindow);

    if (!d->handle)
        return QPoint();

    return d->handle->shadowOffset();
}

QColor DMainWindow::shadowColor() const
{
    D_DC(DMainWindow);

    if (!d->handle)
        return QColor();

    return d->handle->shadowColor();
}

QPainterPath DMainWindow::clipPath() const
{
    D_DC(DMainWindow);

    if (!d->handle)
        return QPainterPath();

    return d->handle->clipPath();
}

QRegion DMainWindow::frameMask() const
{
    D_DC(DMainWindow);

    if (!d->handle)
        return QRegion();

    return d->handle->frameMask();
}

QMargins DMainWindow::frameMargins() const
{
    D_DC(DMainWindow);

    if (!d->handle)
        return QMargins();

    return d->handle->frameMargins();
}

bool DMainWindow::translucentBackground() const
{
    D_DC(DMainWindow);

    if (!d->handle)
        return false;

    return d->handle->translucentBackground();
}

bool DMainWindow::enableSystemResize() const
{
    D_DC(DMainWindow);

    if (!d->handle)
        return false;

    return d->handle->enableSystemResize();
}

bool DMainWindow::enableSystemMove() const
{
    D_DC(DMainWindow);

    if (!d->handle)
        return false;

    return d->handle->enableSystemResize();
}

void DMainWindow::setWindowRadius(int windowRadius)
{
    D_D(DMainWindow);

    if (!d->handle)
        return;

    d->handle->setWindowRadius(windowRadius);
}

void DMainWindow::setBorderWidth(int borderWidth)
{
    D_D(DMainWindow);

    if (!d->handle)
        return;

    d->handle->setBorderWidth(borderWidth);
}

void DMainWindow::setBorderColor(const QColor &borderColor)
{
    D_D(DMainWindow);

    if (!d->handle)
        return;

    d->handle->setBorderColor(borderColor);
}

void DMainWindow::setShadowRadius(int shadowRadius)
{
    D_D(DMainWindow);

    if (!d->handle)
        return;

    d->handle->setShadowRadius(shadowRadius);
}

void DMainWindow::setShadowOffset(const QPoint &shadowOffset)
{
    D_D(DMainWindow);

    if (!d->handle)
        return;

    d->handle->setShadowOffset(shadowOffset);
}

void DMainWindow::setShadowColor(const QColor &shadowColor)
{
    D_D(DMainWindow);

    if (!d->handle)
        return;

    d->handle->setShadowColor(shadowColor);
}

void DMainWindow::setClipPath(const QPainterPath &clipPath)
{
    D_D(DMainWindow);

    if (!d->handle)
        return;

    d->handle->setClipPath(clipPath);
}

void DMainWindow::setFrameMask(const QRegion &frameMask)
{
    D_D(DMainWindow);

    if (!d->handle)
        return;

    d->handle->setFrameMask(frameMask);
}

void DMainWindow::setTranslucentBackground(bool translucentBackground)
{
    D_D(DMainWindow);

    if (!d->handle)
        return;

    d->handle->setTranslucentBackground(translucentBackground);
}

void DMainWindow::setEnableSystemResize(bool enableSystemResize)
{
    D_D(DMainWindow);

    if (!d->handle)
        return;

    d->handle->setEnableSystemResize(enableSystemResize);
}

void DMainWindow::setEnableSystemMove(bool enableSystemMove)
{
    D_D(DMainWindow);

    if (!d->handle)
        return;

    d->handle->setEnableSystemMove(enableSystemMove);
}

DMainWindow::DMainWindow(DMainWindowPrivate &dd, QWidget *parent)
    : QMainWindow(parent)
    , DObject(dd)
{
    d_func()->init();
}

DWIDGET_END_NAMESPACE
