#ifndef DPLATFORMWINDOWHANDLE_H
#define DPLATFORMWINDOWHANDLE_H

#include "dwidget_global.h"

#include <QObject>
#include <QPainterPath>
#include <QColor>
#include <QRegion>

QT_BEGIN_NAMESPACE
class QWindow;
class QWidget;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class DPlatformWindowHandle : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int windowRadius READ windowRadius WRITE setWindowRadius)
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
    Q_PROPERTY(int shadowRadius READ shadowRadius WRITE setShadowRadius)
    Q_PROPERTY(QPoint shadowOffset READ shadowOffset WRITE setShadowOffset)
    Q_PROPERTY(QColor shadowColor READ shadowColor WRITE setShadowColor)
    Q_PROPERTY(QPainterPath clipPath READ clipPath WRITE setClipPath)
    Q_PROPERTY(QRegion frameMask READ frameMask WRITE setFrameMask)

public:
    explicit DPlatformWindowHandle(QWindow *window, QObject *parent = 0);
    explicit DPlatformWindowHandle(QWidget *widget, QObject *parent = 0);

    static void enableDXcbForWindow(QWidget *widget);
    static void enableDXcbForWindow(QWindow *window);

    int windowRadius() const;

    int borderWidth() const;
    QColor borderColor() const;

    int shadowRadius() const;
    QPoint shadowOffset() const;
    QColor shadowColor() const;

    QPainterPath clipPath() const;
    QRegion frameMask() const;

public slots:
    void setWindowRadius(int windowRadius);

    void setBorderWidth(int borderWidth);
    void setBorderColor(const QColor &borderColor);

    void setShadowRadius(int shadowRadius);
    void setShadowOffset(const QPoint &shadowOffset);
    void setShadowColor(const QColor &shadowColor);

    void setClipPath(const QPainterPath &clipPath);
    void setFrameMask(const QRegion &frameMask);

private:
    QWindow *m_window;
};

DWIDGET_END_NAMESPACE

Q_DECLARE_METATYPE(QPainterPath)
Q_DECLARE_METATYPE(QRegion)

#endif // DPLATFORMWINDOWHANDLE_H
