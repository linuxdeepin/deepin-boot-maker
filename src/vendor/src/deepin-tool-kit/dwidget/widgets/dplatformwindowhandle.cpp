#include "dplatformwindowhandle.h"

#include "dapplication.h"

#include <QWindow>
#include <QWidget>
#include <QGuiApplication>
#include <QDebug>

DWIDGET_BEGIN_NAMESPACE

#define DEFINE_CONST_CHAR(Name) const char _##Name[] = "_d_" #Name

DEFINE_CONST_CHAR(useDxcb);
DEFINE_CONST_CHAR(netWmStates);
DEFINE_CONST_CHAR(windowRadius);
DEFINE_CONST_CHAR(borderWidth);
DEFINE_CONST_CHAR(borderColor);
DEFINE_CONST_CHAR(shadowRadius);
DEFINE_CONST_CHAR(shadowOffset);
DEFINE_CONST_CHAR(shadowColor);
DEFINE_CONST_CHAR(clipPath);
DEFINE_CONST_CHAR(frameMask);
DEFINE_CONST_CHAR(frameMargins);
DEFINE_CONST_CHAR(translucentBackground);
DEFINE_CONST_CHAR(enableSystemResize);
DEFINE_CONST_CHAR(enableSystemMove);

DPlatformWindowHandle::DPlatformWindowHandle(QWindow *window, QObject *parent)
    : QObject(parent)
    , m_window(window)
{
    enableDXcbForWindow(window);

    window->installEventFilter(this);
}

DPlatformWindowHandle::DPlatformWindowHandle(QWidget *widget, QObject *parent)
    : QObject(parent)
    , m_window(Q_NULLPTR)
{
    enableDXcbForWindow(widget);

    m_window = widget->windowHandle();
    m_window->installEventFilter(this);
}

void DPlatformWindowHandle::enableDXcbForWindow(QWidget *widget)
{
    Q_ASSERT_X(DApplication::isDXcbPlatform(), "DPlatformWindowHandler:",
               "Must call DPlatformWindowHandler::loadDXcbPlugin before");

    QWidget *window = widget->window();

    if (window->windowHandle()) {
        Q_ASSERT_X(window->windowHandle()->property(_useDxcb).toBool(), "DPlatformWindowHandler:",
                   "Must be called before window handle has been created. See also QWidget::windowHandle()");
    } else {
        Q_ASSERT_X(!window->windowHandle(), "DPlatformWindowHandler:",
                   "Must be called before window handle has been created. See also QWidget::windowHandle()");

        if (window->parentWidget()) {
            /// TODO: Avoid call parentWidget()->enforceNativeChildren().
            qApp->setAttribute(Qt::AA_DontCreateNativeWidgetSiblings, true);
        }
        window->setAttribute(Qt::WA_NativeWindow);

        Q_ASSERT_X(window->windowHandle(), "DPlatformWindowHandler:",
                   "widget window handle is NULL.");

        window->windowHandle()->setProperty(_useDxcb, true);
    }
}

void DPlatformWindowHandle::enableDXcbForWindow(QWindow *window)
{
    Q_ASSERT_X(DApplication::isDXcbPlatform(), "DPlatformWindowHandler:",
               "Must call DPlatformWindowHandler::loadDXcbPlugin before");

    if (window->handle()) {
        Q_ASSERT_X(window->property(_useDxcb).toBool(), "DPlatformWindowHandler:",
                   "Must be called before window handle has been created. See also QWindow::handle()");
    } else {
        Q_ASSERT_X(!window->handle(), "DPlatformWindowHandler:",
                   "Must be called before window handle has been created. See also QWindow::handle()");

        window->setProperty(_useDxcb, true);
    }
}

bool DPlatformWindowHandle::isEnabledDXcb(QWidget *widget)
{
    return widget->windowHandle() && widget->windowHandle()->property(_useDxcb).toBool();
}

bool DPlatformWindowHandle::isEnabledDXcb(QWindow *window)
{
    return window->property(_useDxcb).toBool();
}

int DPlatformWindowHandle::windowRadius() const
{
    return m_window->property(_windowRadius).toInt();
}

int DPlatformWindowHandle::borderWidth() const
{
    return m_window->property(_borderWidth).toInt();
}

QColor DPlatformWindowHandle::borderColor() const
{
    return qvariant_cast<QColor>(m_window->property(_borderColor));
}

int DPlatformWindowHandle::shadowRadius() const
{
    return m_window->property(_shadowRadius).toInt();
}

QPoint DPlatformWindowHandle::shadowOffset() const
{
    return m_window->property(_shadowOffset).toPoint();
}

QColor DPlatformWindowHandle::shadowColor() const
{
    return qvariant_cast<QColor>(m_window->property(_shadowColor));
}

QPainterPath DPlatformWindowHandle::clipPath() const
{
    return qvariant_cast<QPainterPath>(m_window->property(_clipPath));
}

QRegion DPlatformWindowHandle::frameMask() const
{
    return qvariant_cast<QRegion>(m_window->property(_frameMask));
}

QMargins DPlatformWindowHandle::frameMargins() const
{
    return qvariant_cast<QMargins>(m_window->property(_frameMargins));
}

bool DPlatformWindowHandle::translucentBackground() const
{
    return m_window->property(_translucentBackground).toBool();
}

bool DPlatformWindowHandle::enableSystemResize() const
{
    return m_window->property(_enableSystemResize).toBool();
}

bool DPlatformWindowHandle::enableSystemMove() const
{
    return m_window->property(_enableSystemMove).toBool();
}

void DPlatformWindowHandle::setWindowRadius(int windowRadius)
{
    m_window->setProperty(_windowRadius, windowRadius);
}

void DPlatformWindowHandle::setBorderWidth(int borderWidth)
{
    m_window->setProperty(_borderWidth, borderWidth);
}

void DPlatformWindowHandle::setBorderColor(const QColor &borderColor)
{
    m_window->setProperty(_borderColor, QVariant::fromValue(borderColor));
}

void DPlatformWindowHandle::setShadowRadius(int shadowRadius)
{
    m_window->setProperty(_shadowRadius, shadowRadius);
}

void DPlatformWindowHandle::setShadowOffset(const QPoint &shadowOffset)
{
    m_window->setProperty(_shadowOffset, shadowOffset);
}

void DPlatformWindowHandle::setShadowColor(const QColor &shadowColor)
{
    m_window->setProperty(_shadowColor, QVariant::fromValue(shadowColor));
}

void DPlatformWindowHandle::setClipPath(const QPainterPath &clipPath)
{
    m_window->setProperty(_clipPath, QVariant::fromValue(clipPath));
}

void DPlatformWindowHandle::setFrameMask(const QRegion &frameMask)
{
    m_window->setProperty(_frameMask, QVariant::fromValue(frameMask));
}

void DPlatformWindowHandle::setTranslucentBackground(bool translucentBackground)
{
    m_window->setProperty(_translucentBackground, translucentBackground);
}

void DPlatformWindowHandle::setEnableSystemResize(bool enableSystemResize)
{
    m_window->setProperty(_enableSystemResize, enableSystemResize);
}

void DPlatformWindowHandle::setEnableSystemMove(bool enableSystemMove)
{
    m_window->setProperty(_enableSystemMove, enableSystemMove);
}

bool DPlatformWindowHandle::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_window) {
        if (event->type() == QEvent::DynamicPropertyChange) {
            QDynamicPropertyChangeEvent *e = static_cast<QDynamicPropertyChangeEvent*>(event);

            if (e->propertyName() == _windowRadius) {
                emit windowRadiusChanged();
            } else if (e->propertyName() == _borderWidth) {
                emit borderWidthChanged();
            } else if (e->propertyName() == _borderColor) {
                emit borderColorChanged();
            } else if (e->propertyName() == _shadowRadius) {
                emit shadowRadiusChanged();
            } else if (e->propertyName() == _shadowOffset) {
                emit shadowOffsetChanged();
            } else if (e->propertyName() == _shadowColor) {
                emit shadowColorChanged();
            } else if (e->propertyName() == _clipPath) {
                emit clipPathChanged();
            } else if (e->propertyName() == _frameMask) {
                emit frameMaskChanged();
            } else if (e->propertyName() == _frameMargins) {
                emit frameMarginsChanged();
            }
        }
    }

    return false;
}

DWIDGET_END_NAMESPACE
