#include "dplatformwindowhandle.h"

#include "dapplication.h"

#include <QWindow>
#include <QWidget>
#include <QGuiApplication>
#include <QDebug>

DWIDGET_BEGIN_NAMESPACE

#define DEFINE_CONST_CHAR(Name) const char _##Name[] = #Name

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

DPlatformWindowHandle::DPlatformWindowHandle(QWindow *window, QObject *parent)
    : QObject(parent)
    , m_window(window)
{
    enableDXcbForWindow(window);
}

DPlatformWindowHandle::DPlatformWindowHandle(QWidget *widget, QObject *parent)
    : QObject(parent)
    , m_window(Q_NULLPTR)
{
    enableDXcbForWindow(widget);

    m_window = widget->windowHandle();
}

void DPlatformWindowHandle::enableDXcbForWindow(QWidget *widget)
{
    Q_ASSERT_X(DApplication::isDXcbPlatform(), "DPlatformWindowHandler:",
               "Must call DPlatformWindowHandler::loadDXcbPlugin before");

    QWidget *window = widget->window();

    Q_ASSERT_X(!window->windowHandle(), "DPlatformWindowHandler::DPlatformWindowHandler",
               "Must be called before window handle has been created. See also QWidget::windowHandle()");

    window->setAttribute(Qt::WA_NativeWindow);

    Q_ASSERT_X(window->windowHandle(), "DPlatformWindowHandler::DPlatformWindowHandler",
               "widget window handle is NULL.");

    window->windowHandle()->setProperty(_useDxcb, true);
}

void DPlatformWindowHandle::enableDXcbForWindow(QWindow *window)
{
    Q_ASSERT_X(DApplication::isDXcbPlatform(), "DPlatformWindowHandler:",
               "Must call DPlatformWindowHandler::loadDXcbPlugin before");

    Q_ASSERT_X(!window->handle(), "DPlatformWindowHandler:",
               "Must be called before window handle has been created. See also QWindow::handle()");

    window->setProperty(_useDxcb, true);
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

DWIDGET_END_NAMESPACE
