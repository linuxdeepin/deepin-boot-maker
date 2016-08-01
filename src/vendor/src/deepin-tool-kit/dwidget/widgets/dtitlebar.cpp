#include "dtitlebar.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QApplication>

#include <private/dobject_p.h>

#include "dwindowclosebutton.h"
#include "dwindowmaxbutton.h"
#include "dwindowminbutton.h"
#include "dwindowrestorebutton.h"
#include "dwindowoptionbutton.h"
#include "dlabel.h"

#include  "dmenu.h"

DWIDGET_BEGIN_NAMESPACE

const int DefaultTitlebarHeight = 32;
const int DefaultIconHeight = 20;
const int DefaultIconWidth = 20;

class DTitlebarPrivate : public DObjectPrivate
{
protected:
    DTitlebarPrivate(DTitlebar *qq);

private:
    void init();

    QHBoxLayout         *mainLayout;
    DLabel              *iconLabel;
    DLabel              *titleLabel;
    DWindowMinButton    *minButton;
    DWindowMaxButton    *maxButton;
    DWindowCloseButton  *closeButton;
    DWindowOptionButton *optionButton;

    QWidget             *coustomAtea;
    QWidget             *buttonArea;
    QWidget             *titleArea;
    QWidget             *titlePadding;

    DMenu               *menu;
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

    mainLayout->setContentsMargins(5, 2, 5, 0);
    mainLayout->setSpacing(0);

    iconLabel->setFixedSize(DefaultIconWidth, DefaultIconHeight);
    titleLabel->setText(qApp->applicationName());
    // TODO: use QSS
    titleLabel->setStyleSheet("font-size: 14px;");
    titleLabel->setContentsMargins(0, 0, DefaultIconWidth + 10, 0);
//    q->setStyleSheet("background-color: green;");

    QHBoxLayout *buttonAreaLayout = new QHBoxLayout;
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
    q->connect(closeButton, &DWindowCloseButton::clicked, q, &DTitlebar::closeClicked);
    q->connect(maxButton, &DWindowMaxButton::maximum, q, &DTitlebar::maximumClicked);
    q->connect(maxButton, &DWindowMaxButton::restore, q, &DTitlebar::restoreClicked);
    q->connect(minButton, &DWindowMaxButton::clicked, q, &DTitlebar::minimumClicked);
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

///
/// \brief setWindowFlags
/// \param type
/// accpet  WindowTitleHint, WindowSystemMenuHint, WindowMinimizeButtonHint, WindowMaximizeButtonHint
/// and WindowMinMaxButtonsHint.
void DTitlebar::setWindowFlags(Qt::WindowFlags type)
{
    D_D(DTitlebar);
    d->titleLabel->setVisible(type & Qt::WindowTitleHint);
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
    d->coustomAtea->setLayout(l);
    d->buttonArea->resize(old);
}

void DTitlebar::setFixedHeight(int h)
{
    D_D(DTitlebar);
    QWidget::setFixedHeight(h);
    d->coustomAtea->setFixedHeight(h);
    d->buttonArea->setFixedHeight(h);
}

void DTitlebar::setTitle(const QString &title)
{
    D_D(DTitlebar);
    d->titleLabel->setText(title);
}

void DTitlebar::setIcon(const QPixmap &icon)
{
    D_D(DTitlebar);
    d->titleLabel->setContentsMargins(0, 0, 0, 0);
    d->iconLabel->setPixmap(icon.scaled(DefaultIconWidth, DefaultIconHeight, Qt::KeepAspectRatio));
}

void DTitlebar::setWindowState(Qt::WindowState windowState)
{
    D_D(DTitlebar);
    d->maxButton->setWindowState(windowState);
}

int DTitlebar::buttonAreaWidth() const
{
    D_DC(DTitlebar);
    return d->buttonArea->width();
}

void DTitlebar::mouseMoveEvent(QMouseEvent */*event*/)
{
    emit mouseMoving();
}

void DTitlebar::mouseDoubleClickEvent(QMouseEvent */*event*/)
{
    emit doubleClicked();
}


DWIDGET_END_NAMESPACE
