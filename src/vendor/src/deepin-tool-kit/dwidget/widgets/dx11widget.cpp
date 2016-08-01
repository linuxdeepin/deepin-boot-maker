#include "dx11widget.h"

#include <QDebug>
#include <QTimer>
#include <QWidget>
#include <QMouseEvent>
#include <QLayout>
#include <QTemporaryFile>
#include <QImage>


#include <QGraphicsDropShadowEffect>
#include <QGraphicsEffect>

#include <DObjectPrivate>
#include <DGraphicsDropShadowEffect>
#include <DTitlebar>

#include "private/dwidget_p.h"

#ifdef Q_OS_LINUX
#include "../platforms/x11/xutil.h"
#endif

DWIDGET_BEGIN_NAMESPACE

const int WindowGlowRadius = 40;
const int WindowRadius = 4;
const int WindowBorder = 1;
const int WindowHandleWidth = 10;

const QColor BorderColor = QColor(0, 0, 0, 60);
const QColor ShadowColor = QColor(0, 0, 0, 40);
const QColor BackgroundTopColor = QColor(255, 255, 255);
const QColor BackgroundBottonColor = QColor(255, 255, 255);

const QColor TipsBorderColor = QColor(255, 255, 255, 255 * 0.2);
const QColor TipsBackground = QColor(0, 0, 0);

DX11WidgetPrivate::DX11WidgetPrivate(DX11Widget *q) : DObjectPrivate(q)
{
    leftPressed = false;
    resizable = true;
}

void DX11WidgetPrivate::init()
{
    D_Q(DX11Widget);

    dwindowFlags = Qt::Window | Qt::WindowTitleHint |
                   Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint |
                   Qt::WindowCloseButtonHint | Qt::WindowFullscreenButtonHint;
    m_NormalShadowWidth = WindowGlowRadius;
    m_ShadowWidth = WindowGlowRadius;
    m_Radius = WindowRadius;
    m_Border = WindowBorder;
    m_ResizeHandleWidth = WindowHandleWidth;
    m_MousePressed = false;
    m_Shadow = nullptr;
    m_backgroundColor = BackgroundTopColor;

    rootLayout = new QVBoxLayout(q);
    rootLayout->setContentsMargins(WindowGlowRadius,
                                   WindowGlowRadius + m_Border,
                                   WindowGlowRadius,
                                   WindowGlowRadius);
    rootLayout->setSpacing(0);

    windowWidget = new QWidget;
    QVBoxLayout *windowLayout = new QVBoxLayout(windowWidget);
    windowLayout->setMargin(0);
    windowLayout->setSpacing(0);

    rootLayout->addWidget(windowWidget);

    titlebar = new DTitlebar;
    contentWidget = new QWidget;
    QVBoxLayout *contentWidgetLayout = new QVBoxLayout;
    contentWidgetLayout->setSpacing(0);
    contentWidgetLayout->setMargin(0);
    contentWidget->setLayout(contentWidgetLayout);
    contentWidget->setContentsMargins(m_Border, 0, m_Border, m_Border);
    windowLayout->addWidget(titlebar);
    windowLayout->addWidget(contentWidget);
    windowLayout->setAlignment(contentWidget, Qt::AlignCenter);

    auto filter = new FilterMouseMove(windowWidget);
    windowWidget->installEventFilter(filter);
    filter->m_rootWidget = q;

    q->connect(titlebar, &DTitlebar::closeClicked, q, &DX11Widget::close);
    q->connect(titlebar, &DTitlebar::maximumClicked, q, &DX11Widget::showMaximized);
    q->connect(titlebar, &DTitlebar::restoreClicked, q, &DX11Widget::showNormal);
    q->connect(titlebar, &DTitlebar::minimumClicked, q, &DX11Widget::showMinimized);
    q->connect(titlebar, &DTitlebar::optionClicked, q, &DX11Widget::optionClicked);
    q->connect(titlebar, &DTitlebar::mouseMoving, q, &DX11Widget::moveWindow);
    q->connect(titlebar, &DTitlebar::doubleClicked, q, &DX11Widget::showMaximized);
}

QSize DX11WidgetPrivate::externSize(const QSize &size) const
{
    D_QC(DX11Widget);
    return QSize(size.width() + (m_ShadowWidth + m_Border) * 2,
                 size.height() + (m_ShadowWidth + m_Border) * 2);
}

QMargins DX11WidgetPrivate::externMargins() const
{
    return QMargins(m_ShadowWidth + m_Border, m_ShadowWidth + m_Border,
                    m_ShadowWidth + m_Border, m_ShadowWidth + m_Border);
}

int DX11WidgetPrivate::externWidth() const
{
    return m_ShadowWidth + m_Border;
}

DX11Widget::DX11Widget(QWidget *parent): DX11Widget(*new DX11WidgetPrivate(this), parent)
{

}

DX11Widget::DX11Widget(DObjectPrivate &dd, QWidget *parent)
    : QWidget(parent), DObject(dd)
{
    D_D(DX11Widget);
    d->init();
    QWidget::setMouseTracking(true);

    QWidget::setAttribute(Qt::WA_TranslucentBackground, true);
    QWidget::setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    setWindowFlags(windowFlags());

    setShadow();
    DX11Widget::adjustSize();
#ifdef Q_OS_LINUX
    XUtils::SetMouseTransparent(this, true);
#endif
}

void DX11Widget::enterEvent(QEvent *e)
{
//    qDebug() << "enterEvent" ;
    return QWidget::enterEvent(e);
}

void DX11Widget::leaveEvent(QEvent *e)
{
//    qDebug() << "leaveEvent";
    return QWidget::leaveEvent(e);
}

void DX11Widget::mouseMoveEvent(QMouseEvent *event)
{
    D_D(DX11Widget);

    const int x = event->x();
    const int y = event->y();
#ifdef Q_OS_LINUX
    if (d->resizingCornerEdge == XUtils::CornerEdge::kInvalid && d->resizable) {
        XUtils::UpdateCursorShape(this, x, y, d->externMargins(), d->m_ResizeHandleWidth);
    }
#endif

    return QWidget::mouseMoveEvent(event);
}

void DX11Widget::mousePressEvent(QMouseEvent *event)
{
    D_D(DX11Widget);

    const int x = event->x();
    const int y = event->y();
    if (event->button() == Qt::LeftButton) {
#ifdef Q_OS_LINUX
        const XUtils::CornerEdge ce = XUtils::GetCornerEdge(this, x, y, d->externMargins(), d->m_ResizeHandleWidth);
        if (ce != XUtils::CornerEdge::kInvalid) {
            d->resizingCornerEdge = ce;
            XUtils::StartResizing(this, QCursor::pos(), ce);
        }
#endif
    }

    return QWidget::mousePressEvent(event);
}

void DX11Widget::mouseReleaseEvent(QMouseEvent *event)
{
    D_D(DX11Widget);
#ifdef Q_OS_LINUX
    d->resizingCornerEdge = XUtils::CornerEdge::kInvalid;
#endif
    return QWidget::mouseReleaseEvent(event);
}

void DX11Widget::showMinimized()
{
#ifdef Q_OS_LINUX
    XUtils::ShowMinimizedWindow(this, true);
#endif
    QWidget::showMinimized();
}

void DX11Widget::showMaximized()
{
    D_D(DX11Widget);
    d->m_ShadowWidth = 0;
#ifdef Q_OS_LINUX
    XUtils::ShowMaximizedWindow(this);
#endif
    this->show();
    this->activateWindow();
    this->raise();
}

void DX11Widget::setContentsMargins(int left, int top, int right, int bottom)
{
    D_D(DX11Widget);
    d->contentWidget->setContentsMargins(left, top, right, bottom);
}

void DX11Widget::setContentsMargins(const QMargins &margins)
{
    D_D(DX11Widget);
    d->contentWidget->setContentsMargins(margins);
}

void DX11Widget::getContentsMargins(int *left, int *top, int *right, int *bottom) const
{
    D_DC(DX11Widget);
    d->contentWidget->getContentsMargins(left, top, right, bottom);
}

QMargins DX11Widget::contentsMargins() const
{
    D_DC(DX11Widget);
    return d->contentWidget->contentsMargins();
}

void DX11Widget::showFullScreen()
{
    D_D(DX11Widget);
    d->m_ShadowWidth = 0;

#ifdef Q_OS_LINUX
    XUtils::ShowFullscreenWindow(this, true);
#endif
    this->show();
    this->activateWindow();
    this->raise();
}

void DX11Widget::moveWindow()
{
#ifdef Q_OS_LINUX
    XUtils::MoveWindow(this);
#endif
}

void DX11Widget::toggleMaximizedWindow()
{
#ifdef Q_OS_LINUX
    XUtils::ToggleMaximizedWindow(this);
#endif
}

void DX11Widget::showNormal()
{
    D_D(DX11Widget);
    d->m_ShadowWidth = d->m_NormalShadowWidth;
#ifdef Q_OS_LINUX
    XUtils::ShowNormalWindow(this);
#endif
}


void DX11Widget::removeLayout()
{
    qDeleteAll(this->children());
}

Qt::WindowFlags DX11Widget::windowFlags()
{
    D_D(DX11Widget);
    return d->dwindowFlags;
}

///
/// \brief setWindowFlags
/// \param type
///
void DX11Widget::setWindowFlags(Qt::WindowFlags type)
{
    D_D(DX11Widget);
    d->dwindowFlags = type;
    d->titlebar->setWindowFlags(type);
}

int DX11Widget::titlebarHeight() const
{
    D_DC(DX11Widget);
    return d->titlebar->height();
}

void DX11Widget::setTitlebarFixedHeight(int h)
{
    D_D(DX11Widget);
    d->titlebar->setFixedHeight(h);
    DX11Widget::adjustSize();
}

void DX11Widget::setTitle(const QString &t)
{
    D_D(DX11Widget);
    d->titlebar->setTitle(t);
}

void DX11Widget::setTitleIcon(const QPixmap &icon)
{
    D_D(DX11Widget);
    d->titlebar->setIcon(icon);
}

void DX11Widget::setTitlebarMenu(DMenu *menu)
{
    D_D(DX11Widget);
    d->titlebar->setMenu(menu);
}

void DX11Widget::setTitlebarWidget(QWidget *w, bool fixCenterPos)
{
    D_D(DX11Widget);
    d->titlebar->setCustomWidget(w, Qt::AlignCenter, fixCenterPos);
}

// TODO: fix layout
QLayout *DX11Widget::layout() const
{
    D_DC(DX11Widget);
    return d->contentWidget->layout();
}

void DX11Widget::setLayout(QLayout *l)
{
    D_D(DX11Widget);

    qDeleteAll(d->contentWidget->children());
    d->contentWidget->setLayout(l);
    d->contentWidget->adjustSize();
    d->windowWidget->adjustSize();
    DX11Widget::resize(d->windowWidget->size());
}

int DX11Widget::radius() const
{
    D_DC(DX11Widget);
    return d->m_Radius;
}

void DX11Widget::setRadius(int r)
{
    D_D(DX11Widget);
    if (r > d->m_Border * 2) {
        r = d->m_Border * 2;
    }
    d->m_Radius = r;
}

int DX11Widget::shadowWidth() const
{
    D_DC(DX11Widget);
    return d->m_Radius;
}

void DX11Widget::setShadowWidth(int r)
{
    D_D(DX11Widget);
    d->m_Radius = r;
}

QColor DX11Widget::backgroundColor() const
{
    D_DC(DX11Widget);

    return d->m_backgroundColor;
}

void DX11Widget::setBackgroundColor(QColor backgroundColor)
{
    D_D(DX11Widget);

    if (d->m_backgroundColor == backgroundColor) {
        return;
    }

    d->m_backgroundColor = backgroundColor;
    emit backgroundColorChanged(backgroundColor);
}

int DX11Widget::border() const
{
    D_DC(DX11Widget);
    return d->m_Border;
}

void DX11Widget::setBorder(int b)
{
    D_D(DX11Widget);
    if (b < 0) { b = 0; }
    d->m_Border = b;
    if (d->m_Radius > b * 2) {
        d->m_Radius = b * 2;
    }
}

const QPixmap &DX11Widget::backgroundImage() const
{
    D_DC(DX11Widget);
    return d->m_Background;
}

void DX11Widget::setBackgroundImage(const QPixmap &bk)
{
    D_D(DX11Widget);

    QImage bkImage = bk.toImage();
    QPixmap maskPixmap(bkImage.size());
    maskPixmap.fill(Qt::transparent);
    QPainterPath path;
    path.addRoundRect(QRectF(0, 0, bkImage.width(), bkImage.height()), 2);
    QPainter bkPainter(&maskPixmap);
    bkPainter.setRenderHint(QPainter::Antialiasing);
    bkPainter.setPen(QPen(Qt::black, 1));
    bkPainter.fillPath(path, QBrush(Qt::red));

    QImage maskImage = maskPixmap.toImage();
    int nDepth = bkImage.depth();
    int nWidth = maskImage.width();
    int nHeight = maskImage.height();
    switch (nDepth) {
    case 32:
        for (int y = 0; y < nHeight; ++y) {
            quint32 *pMaskData = (quint32 *)(maskImage.scanLine(y));
            quint32 *pWaveData = (quint32 *)(bkImage.scanLine(y));
            quint32 alpha;
            for (int x = 0; x < nWidth; ++x) {
                alpha = (pMaskData[x] >> 24) << 24 | 0x00FFFFFF;
                pMaskData[x] = pWaveData[x] & (alpha);
            }
        }
        break;
    default:
        break;
    }

    // TODO: fixme , only export as png file can work
    QTemporaryFile bkTmp;
    maskImage.save(&bkTmp, "png");
    bkTmp.close();

    d->m_Background = QPixmap(bkTmp.fileName());
}

void DX11Widget::setFixedSize(const QSize &size)
{
    D_D(DX11Widget);
    d->resizable = false;
    d->titlebar->setMinimumWidth(size.width());
    d->windowWidget->setFixedSize(size);
    d->contentWidget->setFixedSize(size.width(), size.height() - titlebarHeight());
    QSize externSize = d->externSize(size);
    QWidget::setFixedSize(externSize);
    setWindowFlags(windowFlags() & ~ Qt::WindowMaximizeButtonHint);

#ifdef Q_OS_LINUX
    XUtils::DisableResize(this);
#endif
}

void DX11Widget::setFixedSize(int w, int h)
{
    DX11Widget::setFixedSize(QSize(w, h));
}

void DX11Widget::setFixedWidth(int w)
{
    D_D(DX11Widget);
    d->titlebar->setFixedWidth(w);
    d->windowWidget->setFixedWidth(w);
    QWidget::setFixedWidth(w + d->m_ShadowWidth + d->m_Border);
}

void DX11Widget::resize(const QSize &size)
{
    D_D(DX11Widget);
    d->windowWidget->resize(size);
    QSize externSize = d->externSize(size);
    if (d->resizable) {
        QWidget::setMinimumSize(externSize);
        QWidget::resize(externSize);
    } else {
        QWidget::setMaximumSize(externSize);
    }
}

void DX11Widget::adjustSize()
{
    D_D(DX11Widget);
    d->windowWidget->adjustSize();
    QSize externSize = d->externSize(d->windowWidget->size());
    if (d->resizable) {
        QWidget::setMinimumSize(externSize);
        QWidget::resize(externSize);
    } else {
        QWidget::setMaximumSize(externSize);
        QWidget::resize(externSize);
    }
}

QSize DX11Widget::size() const
{
    D_DC(DX11Widget);
    return d->windowWidget->size();
}


void DX11Widget::move(int x, int y)
{
    D_D(DX11Widget);
    QWidget::move(x - d->externWidth(), y - d->externWidth());
}

void DX11Widget::move(const QPoint &p)
{
    DX11Widget::move(p.x(), p.y());
}

QRect DX11Widget::frameGeometry() const
{
    D_DC(DX11Widget);
    return d->windowWidget->frameGeometry();
}

const QRect &DX11Widget::geometry() const
{
    D_DC(DX11Widget);
    return d->windowWidget->geometry();
}

QRect DX11Widget::normalGeometry() const
{
    D_DC(DX11Widget);
    return d->windowWidget->normalGeometry();
}

int DX11Widget::x() const
{
    D_DC(DX11Widget);
    return d->windowWidget->x();
}

int DX11Widget::y() const
{
    D_DC(DX11Widget);
    return d->windowWidget->y();
}

QPoint DX11Widget::pos() const
{
    D_DC(DX11Widget);
    return d->windowWidget->pos();
}

QSize DX11Widget::frameSize() const
{
    D_DC(DX11Widget);
    return d->windowWidget->frameSize();
}

int DX11Widget::width() const
{
    D_DC(DX11Widget);
    return d->windowWidget->width();
}

int DX11Widget::height() const
{
    D_DC(DX11Widget);
    return d->windowWidget->height();
}

inline QRect DX11Widget::rect() const
{
    D_DC(DX11Widget);
    return d->windowWidget->rect();
}

QRect DX11Widget::childrenRect() const
{
    D_DC(DX11Widget);
    return d->windowWidget->childrenRect();
}

QRegion DX11Widget::childrenRegion() const
{
    D_DC(DX11Widget);
    return d->windowWidget->childrenRegion();
}

void DX11Widget::showEvent(QShowEvent *e)
{
    D_D(DX11Widget);
    QWidget::showEvent(e);
}

void DX11Widget::resizeEvent(QResizeEvent *e)
{
    D_D(DX11Widget);
    int resizeHandleWidth = d->resizable ? d->m_ResizeHandleWidth : 0;
#ifdef Q_OS_LINUX
    XUtils::SetWindowExtents(this, d->externWidth(), resizeHandleWidth);
#endif
    QWidget::resizeEvent(e);
}

void DX11Widget::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent(e);
}

void DX11Widget::setShadow()
{
    D_D(DX11Widget);
    d->m_Shadow = new QGraphicsDropShadowEffect(this);
    d->m_Shadow->setColor(ShadowColor);
    d->m_Shadow->setOffset(0, 10);
    d->m_Shadow->setBlurRadius(d->m_ShadowWidth);
    this->setGraphicsEffect(d->m_Shadow);
}

void DX11Widget::paintEvent(QPaintEvent * /*e*/)
{
    D_D(DX11Widget);
    int radius = d->m_Radius;
    //. TODO: border not  part of window?
    int windowExtern = d->m_ShadowWidth + d->m_Border;
    int contentExtern = d->m_ShadowWidth + d->m_Border;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QRect contentRect = QWidget::rect().marginsRemoved(QMargins(contentExtern, contentExtern, contentExtern, contentExtern));
    QRect windowRect = QWidget::rect().marginsRemoved(QMargins(windowExtern, windowExtern, windowExtern, windowExtern));

    if (! d->m_Background.isNull()) {
        painter.drawPixmap(contentRect, d->m_Background);
    } else {
        QPoint topLeft(windowRect.x(), windowRect.y());
        QPoint bottomRight(windowRect.x() + windowRect.width(), windowRect.y() + windowRect.height());
        QPainterPath border;
        border.addRoundedRect(windowRect, radius, radius);

        QLinearGradient linearGradient(topLeft, QPoint(topLeft.x(), bottomRight.y()));
        linearGradient.setColorAt(0.0, d->m_backgroundColor);
        linearGradient.setColorAt(0.2, d->m_backgroundColor);
        linearGradient.setColorAt(1.0, d->m_backgroundColor);

        QPen borderPen(BorderColor);
        painter.setBrush(QBrush(linearGradient));
        painter.strokePath(border, borderPen);
        painter.fillPath(border, /*palette().background()*/QBrush(linearGradient));
    }
}

FilterMouseMove::FilterMouseMove(QObject *object) : QObject(object)
{

}

FilterMouseMove::~FilterMouseMove()
{

}

bool FilterMouseMove::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type()) {
    case QEvent::Enter: {
        if (qobject_cast<QWidget *>(obj) != qobject_cast<QWidget *>(this->parent())) {
            break;
        }
        if (m_rootWidget) {
#ifdef Q_OS_LINUX
            XUtils::ResetCursorShape(m_rootWidget);
#endif
        }
        break;
    }
    default: {
    }
    }
    return QObject::eventFilter(obj, event);
}

DWIDGET_END_NAMESPACE
