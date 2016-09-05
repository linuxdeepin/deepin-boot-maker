#include "dtitlebar.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QApplication>
#include <QMouseEvent>

#include <private/dobject_p.h>

#include "dwindowclosebutton.h"
#include "dwindowmaxbutton.h"
#include "dwindowminbutton.h"
#include "dwindowrestorebutton.h"
#include "dwindowoptionbutton.h"
#include "dlabel.h"
#include "dplatformwindowhandle.h"
#ifdef Q_OS_LINUX
#include "../platforms/x11/xutil.h"
#endif

#include  "dmenu.h"

DWIDGET_BEGIN_NAMESPACE

const int DefaultTitlebarHeight = 40;
const int DefaultIconHeight = 20;
const int DefaultIconWidth = 20;

class DTitlebarPrivate : public DObjectPrivate
{
protected:
    DTitlebarPrivate(DTitlebar *qq);

private:
    void init();
    void _q_toggleWindowState();
    void _q_showMinimized();

    QHBoxLayout         *mainLayout;
    DLabel              *iconLabel;
    DLabel              *titleLabel;
    DWindowMinButton    *minButton;
    DWindowMaxButton    *maxButton;
    DWindowCloseButton  *closeButton;
    DWindowOptionButton *optionButton;

    QWidget             *customWidget = Q_NULLPTR;
    QWidget             *coustomAtea;
    QWidget             *buttonArea;
    QWidget             *titleArea;
    QWidget             *titlePadding;
    QLabel              *separator;

    DMenu               *menu;

    QWidget             *parentWindow = Q_NULLPTR;

    bool                mousePressed = false;

    Q_DECLARE_PUBLIC(DTitlebar)
};

DTitlebarPrivate::DTitlebarPrivate(DTitlebar *qq): DObjectPrivate(qq)
{
}

void DTitlebarPrivate::init()
{
    D_Q(DTitlebar);

    mainLayout      = new QHBoxLayout;
    iconLabel       = new DLabel;
    titleLabel      = new DLabel;
    minButton       = new DWindowMinButton;
    maxButton       = new DWindowMaxButton;
    closeButton     = new DWindowCloseButton;
    optionButton    = new DWindowOptionButton;
    coustomAtea     = new QWidget;
    buttonArea      = new QWidget;
    titleArea       = new QWidget;
    titlePadding    = new QWidget;
    separator       = new QLabel(q);

    mainLayout->setContentsMargins(6, 0, 6, 0);
    mainLayout->setSpacing(0);

    iconLabel->setFixedSize(DefaultIconWidth, DefaultIconHeight);
    titleLabel->setText(qApp->applicationName());
    // TODO: use QSS
    titleLabel->setStyleSheet("font-size: 14px;");
    titleLabel->setContentsMargins(0, 0, DefaultIconWidth + 10, 0);
//    q->setStyleSheet("background-color: green;");

    separator->setFixedHeight(1);
    separator->setStyleSheet("background: rgba(0, 0, 0, 20);");
    separator->hide();

    QHBoxLayout *buttonAreaLayout = new QHBoxLayout;
	buttonAreaLayout->setContentsMargins(0, 1, 0, 0);
    buttonAreaLayout->setMargin(0);
    buttonAreaLayout->setSpacing(0);
    buttonAreaLayout->addWidget(optionButton);
    buttonAreaLayout->addWidget(minButton);
    buttonAreaLayout->addWidget(maxButton);
    buttonAreaLayout->addWidget(closeButton);
    buttonArea->setLayout(buttonAreaLayout);

    QHBoxLayout *titleAreaLayout = new QHBoxLayout;
    titleAreaLayout->setMargin(0);
    titleAreaLayout->setSpacing(0);
    titlePadding->setFixedSize(buttonArea->size());
    titleAreaLayout->addWidget(titlePadding);
    titleAreaLayout->addStretch();
    titleAreaLayout->addWidget(iconLabel);
    titleAreaLayout->setAlignment(iconLabel, Qt::AlignCenter);
    titleAreaLayout->addSpacing(10);
    titleAreaLayout->addWidget(titleLabel);
    titleAreaLayout->setAlignment(titleLabel, Qt::AlignCenter);

    titleAreaLayout->addStretch();
    titleArea->setLayout(titleAreaLayout);

    QHBoxLayout *coustomAteaLayout = new QHBoxLayout;
    coustomAteaLayout->setMargin(0);
    coustomAteaLayout->setSpacing(0);
    coustomAteaLayout->addWidget(titleArea);
    coustomAtea->setLayout(coustomAteaLayout);

    mainLayout->addWidget(coustomAtea);
    mainLayout->addWidget(buttonArea);
    mainLayout->setAlignment(buttonArea, Qt::AlignRight |  Qt::AlignVCenter);

    q->setLayout(mainLayout);
    q->setFixedHeight(DefaultTitlebarHeight);
    q->setMinimumHeight(DefaultTitlebarHeight);
    coustomAtea->setFixedHeight(q->height());
    buttonArea->setFixedHeight(q->height());

    q->connect(optionButton, &DWindowOptionButton::clicked, q, &DTitlebar::optionClicked);
}

void DTitlebarPrivate::_q_toggleWindowState()
{
    D_QC(DTitlebar);

    QWidget *parentWindow = q->parentWidget();

    if (!parentWindow) {
        return;
    }

    parentWindow = parentWindow->window();

    if (parentWindow->isMaximized()) {
        parentWindow->showNormal();
    } else if (!parentWindow->isFullScreen()) {
        parentWindow->showMaximized();
    }
}

void DTitlebarPrivate::_q_showMinimized()
{
    if (DPlatformWindowHandle::isEnabledDXcb(parentWindow)) {
        parentWindow->showMinimized();
    } else {
#ifdef Q_OS_LINUX
        XUtils::ShowMinimizedWindow(parentWindow, true);
#else
        parentWindow->showMinimized();
#endif
    }
}

DTitlebar::DTitlebar(QWidget *parent) :
    QWidget(parent),
    DObject(*new DTitlebarPrivate(this))
{
    D_D(DTitlebar);
    d->init();
    d->buttonArea->adjustSize();
    d->buttonArea->resize(d->buttonArea->size());
    d->titlePadding->setFixedSize(d->buttonArea->size());
}

DMenu *DTitlebar::menu() const
{
    D_DC(DTitlebar);

    return d->menu;
}

QWidget *DTitlebar::customWidget() const
{
    D_DC(DTitlebar);

    return d->customWidget;
}

///
/// \brief setWindowFlags
/// \param type
/// accpet  WindowTitleHint, WindowSystemMenuHint, WindowMinimizeButtonHint, WindowMaximizeButtonHint
/// and WindowMinMaxButtonsHint.
void DTitlebar::setWindowFlags(Qt::WindowFlags type)
{
    D_D(DTitlebar);
    if (d->titleLabel) {
        d->titleLabel->setVisible(type & Qt::WindowTitleHint);
    }
    d->iconLabel->setVisible(type & Qt::WindowTitleHint);
    d->minButton->setVisible(type & Qt::WindowMinimizeButtonHint);
    d->maxButton->setVisible(type & Qt::WindowMaximizeButtonHint);
    d->closeButton->setVisible(type & Qt::WindowCloseButtonHint);
    d->optionButton->setVisible(type & Qt::WindowSystemMenuHint);
    d->buttonArea->adjustSize();
    d->buttonArea->resize(d->buttonArea->size());
    d->titlePadding->setFixedSize(d->buttonArea->size());
}

void DTitlebar::setMenu(DMenu *menu)
{
    D_D(DTitlebar);

    d->menu = menu;
    if (d->menu) {
        disconnect(this, &DTitlebar::optionClicked, 0, 0);
        connect(this, &DTitlebar::optionClicked, this, &DTitlebar::showMenu);
    }
}

void DTitlebar::showMenu()
{
    D_D(DTitlebar);
    d->menu->exec(d->optionButton->mapToGlobal(d->optionButton->rect().bottomLeft()));
}

void DTitlebar::showEvent(QShowEvent *event)
{
    D_D(DTitlebar);
    d->separator->setFixedWidth(width());
    d->separator->move(0, height() - d->separator->height());
    QWidget::showEvent(event);
}

void DTitlebar::mousePressEvent(QMouseEvent *event)
{
    D_D(DTitlebar);
    d->mousePressed = (event->buttons() == Qt::LeftButton);

    emit mousePressed(event->buttons());
}

void DTitlebar::mouseReleaseEvent(QMouseEvent *event)
{
    D_D(DTitlebar);
    if (event->buttons() == Qt::LeftButton) {
        d->mousePressed = false;
    }
}

bool DTitlebar::eventFilter(QObject *obj, QEvent *event)
{
    D_D(DTitlebar);

    if (obj == d->parentWindow) {
        if (event->type() == QEvent::WindowStateChange) {
            d->maxButton->setMaximized(d->parentWindow->windowState() == Qt::WindowMaximized);
        }
    }

    return QWidget::eventFilter(obj, event);
}

void DTitlebar::setCustomWidget(QWidget *w, bool fixCenterPos)
{
    setCustomWidget(w, Qt::AlignCenter, fixCenterPos);
}


void DTitlebar::setCustomWidget(QWidget *w, Qt::AlignmentFlag wflag, bool fixCenterPos)
{
    D_D(DTitlebar);
    if (!w || w == d->titleArea) {
        return;
    }

    QSize old = d->buttonArea->size();

    QHBoxLayout *l = new QHBoxLayout;
    l->setSpacing(0);
    l->setMargin(0);

    if (fixCenterPos) {
        d->titlePadding = new QWidget;
        d->titlePadding->setFixedSize(old);
        l->addWidget(d->titlePadding);
    }

    l->addWidget(w);
    l->setAlignment(w, wflag);
    qDeleteAll(d->coustomAtea->children());
    d->titleLabel = nullptr;
    d->coustomAtea->setLayout(l);
    d->buttonArea->resize(old);
    d->customWidget = w;
}

void DTitlebar::setFixedHeight(int h)
{
    D_D(DTitlebar);
    QWidget::setFixedHeight(h);
    d->coustomAtea->setFixedHeight(h);
    d->buttonArea->setFixedHeight(h);
}

void DTitlebar::setSeparatorVisible(bool visible)
{
    D_D(DTitlebar);
    if (visible) {
        d->separator->show();
        d->separator->raise();
    } else {
        d->separator->hide();
    }
}

void DTitlebar::setTitle(const QString &title)
{
    D_D(DTitlebar);
    if (d->titleLabel) {
        d->titleLabel->setText(title);
    }
}

void DTitlebar::setIcon(const QPixmap &icon)
{
    D_D(DTitlebar);
    if (d->titleLabel) {
        d->titleLabel->setContentsMargins(0, 0, 0, 0);
        d->iconLabel->setPixmap(icon.scaled(DefaultIconWidth, DefaultIconHeight, Qt::KeepAspectRatio));
    }
}

void DTitlebar::setWindowState(Qt::WindowState windowState)
{
    D_D(DTitlebar);
    d->maxButton->setWindowState(windowState);
}

void DTitlebar::toggleWindowState()
{
    D_D(DTitlebar);

    d->_q_toggleWindowState();
}

int DTitlebar::buttonAreaWidth() const
{
    D_DC(DTitlebar);
    return d->buttonArea->width();
}

bool DTitlebar::separatorVisible() const
{
    D_DC(DTitlebar);
    return d->separator->isVisible();
}

void DTitlebar::setVisible(bool visible)
{
    D_D(DTitlebar);

    if (visible == isVisible()) {
        return;
    }

    QWidget::setVisible(visible);

    if (visible) {
        d->parentWindow = parentWidget();

        if (!d->parentWindow) {
            return;
        }

        d->parentWindow = d->parentWindow->window();
        d->parentWindow->installEventFilter(this);

        connect(d->maxButton, SIGNAL(clicked()), this, SLOT(_q_toggleWindowState()));
        connect(this, SIGNAL(doubleClicked()), this, SLOT(_q_toggleWindowState()));
        connect(d->minButton, SIGNAL(clicked()), this, SLOT(_q_showMinimized()));
        connect(d->closeButton, &DWindowCloseButton::clicked, d->parentWindow, &QWidget::close);
    } else {
        if (!d->parentWindow) {
            return;
        }

        d->parentWindow->removeEventFilter(this);

        disconnect(d->maxButton, SIGNAL(clicked()), this, SLOT(_q_toggleWindowState()));
        disconnect(this, SIGNAL(doubleClicked()), this, SLOT(_q_toggleWindowState()));
        disconnect(d->minButton, SIGNAL(clicked()), this, SLOT(_q_showMinimized()));
        disconnect(d->closeButton, &DWindowCloseButton::clicked, d->parentWindow, &QWidget::close);
    }
}

void DTitlebar::mouseMoveEvent(QMouseEvent *event)
{
    D_DC(DTitlebar);

    if (event->buttons() == Qt::LeftButton /*&& d->mousePressed*/) {
        Qt::MouseButton button = event->buttons() & Qt::LeftButton ? Qt::LeftButton : Qt::NoButton;
        emit mouseMoving(button);
    }
    QWidget::mouseMoveEvent(event);
}

void DTitlebar::mouseDoubleClickEvent(QMouseEvent *event)
{
    D_D(DTitlebar);

    if (event->buttons() == Qt::LeftButton) {
        d->mousePressed = false;

        emit doubleClicked();
    }
}

DWIDGET_END_NAMESPACE

#include "moc_dtitlebar.cpp"
