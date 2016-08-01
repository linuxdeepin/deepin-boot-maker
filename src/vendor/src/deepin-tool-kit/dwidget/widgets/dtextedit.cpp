#include "dtextedit.h"
#include "dthememanager.h"
#include "private/dtextedit_p.h"
#include "dscrollbar.h"

#include <QResizeEvent>

DWIDGET_BEGIN_NAMESPACE

DTextEditPrivate::DTextEditPrivate(DTextEdit *qq) :
    DObjectPrivate(qq)
{

}

void DTextEditPrivate::init()
{
    D_Q(DTextEdit);

    insideFrame = new QFrame(q);
    insideFrame->setObjectName("TextEditInsideFrame");
    insideFrame->setFocusProxy(q);
    insideFrame->setAttribute(Qt::WA_TransparentForMouseEvents);

    q->setContextMenuPolicy(Qt::NoContextMenu);
    q->setVerticalScrollBar(new DScrollBar);
    q->setHorizontalScrollBar(new DScrollBar);
}

DTextEdit::DTextEdit(QWidget *parent) :
    QTextEdit(parent),
    DObject(*new DTextEditPrivate(this))
{
    D_THEME_INIT_WIDGET(DTextEdit);

    d_func()->init();
}

void DTextEdit::resizeEvent(QResizeEvent *event)
{
    QTextEdit::resizeEvent(event);

    D_D(DTextEdit);

    const QSize &size = event->size();

    d->insideFrame->setFixedHeight(size.height() - 1);
    d->insideFrame->setFixedWidth(size.width());
}

DWIDGET_END_NAMESPACE
