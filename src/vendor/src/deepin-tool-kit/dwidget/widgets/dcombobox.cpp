/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QFrame>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QJsonDocument>
#include <QResizeEvent>

#include "private/dcombobox_p.h"
#include "dcombobox.h"
#include "dthememanager.h"

DWIDGET_BEGIN_NAMESPACE

DComboBoxPrivate::DComboBoxPrivate(DComboBox *parent) :
    DObjectPrivate(parent)
{

}

void DComboBoxPrivate::init()
{
    D_Q(DComboBox);

    initInsideFrame();

    //TODO, just work on ediable
    q->setMaxVisibleItems(MAX_VISIBLE_ITEMS);

    //default view not support ::item sub-control
    q->setView(new QListView());

    q->view()->setObjectName("DComboBoxItemView");
    q->view()->setAutoScroll(true);

    setMaskLabel(new DComboBoxItem(q));

    q->connect(q, SIGNAL(currentIndexChanged(int)), q, SLOT(_q_slotCurrentIndexChange(int)));
    QObject::connect(q->view(), &QAbstractItemView::entered, q, [=] (QModelIndex index) {
        if(hoveredItem)
            hoveredItem->setHovered(false);

        DComboBoxItem *item =  qobject_cast<DComboBoxItem*>(q->view()->indexWidget(index));

        if(item) {
            item->setHovered(true);
            hoveredItem = item;
        }

        hoverIndex = index;
    });
}

//Bypassing the problem here
//qss can't draw box-shadow
void DComboBoxPrivate::initInsideFrame()
{
    D_Q(DComboBox);

    QFrame *insideFrame = new QFrame(q);

    insideFrame->raise();
    insideFrame->setAttribute(Qt::WA_TransparentForMouseEvents);
    insideFrame->setObjectName("DComboxInsideFrame");
    //the sub-widget of QCombobox can't Inherits the style sheet from QCombobox
    //so, read the QCombobox's style sheet and set to it sub-widget
    insideFrame->setStyleSheet(q->styleSheet());

    QHBoxLayout *insideLayout = new QHBoxLayout(q);

    insideLayout->setContentsMargins(OUTSIDE_BORDER_WIDTH, OUTSIDE_BORDER_WIDTH,
                                     OUTSIDE_BORDER_WIDTH, OUTSIDE_BORDER_WIDTH);
    insideLayout->addWidget(insideFrame);
}

void DComboBoxPrivate::restylePopupEnds()
{
    D_Q(DComboBox);

    QList<QWidget *> childs = q->findChildren<QWidget *>();
    QList<QWidget *> ends;  //end of popup frame
    for (QWidget * w : childs) {
        if (QString(w->metaObject()->className()) == "QComboBoxPrivateScroller") {
            ends << w;
            w->setFixedHeight(12);
        }
    }

    for (int i = 0; i < ends.length(); i ++) {
        QWidget *w  = ends.at(i);
        w->parentWidget()->setAttribute(Qt::WA_TranslucentBackground);
        w->setStyleSheet("background: transparent");
        QHBoxLayout *layout = new QHBoxLayout(w);
        layout->setContentsMargins(0, 0, 0, 0);
        QFrame *f = new QFrame(w);
        if (i == 0)
            f->setObjectName("ComboboxPopupTopEnd");
        else
            f->setObjectName("ComboboxPopupBottomEnd");
        f->setStyleSheet(q->styleSheet());
        f->resize(w->size());
        layout->addWidget(f);
    }
}

void DComboBoxPrivate::_q_slotCurrentIndexChange(int index)
{
    D_Q(DComboBox);

    if(q->isEditable()) {
        if(q->currentText().isEmpty()) {
            DComboBoxModel *m = static_cast<DComboBoxModel *>(q->model());

            q->lineEdit()->setText(m->getJsonData(index)["itemText"].toString());
        } else {
            q->lineEdit()->setText(q->currentText());
        }
    } else {
        DComboBoxModel *m = static_cast<DComboBoxModel *>(q->model());
        QWidget *w = q->view()->indexWidget(m->getModelIndex(index));

        if (w) {
            w->setFixedWidth(q->width());

            QRect rect = q->rect();
            rect.setHeight(-1);

            if(checkedItem)
                checkedItem->setChecked(false);

            DComboBoxItem *item = qobject_cast<DComboBoxItem*>(w);

            if(item) {
                item->setChecked(true);
                checkedItem = item;

                maskLabel->setData(item->data());
            } else {
                maskLabel->setPixmap(w->grab(rect));
            }
        }
    }
}

void DComboBoxPrivate::setMaskLabel(DComboBoxItem *label)
{
    D_Q(DComboBox);

    if(maskLabel)
        maskLabel->deleteLater();

    maskLabel = label;
    maskLabel->setObjectName("DComboBoxTitleMask");
    maskLabel->setParent(q);
    maskLabel->setStyleSheet(q->styleSheet());

    maskLabel->setFixedSize(q->size());
}

DComboBoxItem::DComboBoxItem(QWidget *parent) :
    QLabel(parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_TranslucentBackground);
}

bool DComboBoxItem::checked() const
{
    return m_checked;
}

void DComboBoxItem::setChecked(bool value)
{
    if(value == m_checked)
        return;

    m_checked = value;

    style()->unpolish(this);
    style()->polish(this);

    emit checkedChanged(m_checked);
}

bool DComboBoxItem::hovered() const
{
    return m_hovered;
}

void DComboBoxItem::setHovered(bool value)
{
    if(value == m_hovered)
        return;

    m_hovered = value;

    style()->unpolish(this);
    style()->polish(this);

    emit hoveredChanged(m_hovered);
}

QVariantMap DComboBoxItem::data() const
{
    QVariantMap map;

    map["text"] = text();

    return map;
}

void DComboBoxItem::setData(const QVariantMap &map)
{
    if(map.isEmpty())
        return;

    setText(map.values().first().toString());
}

DComboBox::DComboBox(QWidget *parent) :
    QComboBox(parent),
    DObject(*new DComboBoxPrivate(this))
{
    D_THEME_INIT_WIDGET(DComboBox, alert);

    d_func()->init();
    d_func()->restylePopupEnds();
}

void DComboBox::setFixedSize(int w, int h)
{
    D_D(DComboBox);

    QComboBox::setFixedSize(w, h);

    d->maskLabel->setFixedSize(size());
}

void DComboBox::setFixedSize(QSize size)
{
    D_D(DComboBox);

    QComboBox::setFixedSize(size);

    d->maskLabel->setFixedSize(size);
}

QModelIndex DComboBox::hoverIndex() const
{
    D_DC(DComboBox);

    return d->hoverIndex;
}

QString DComboBox::insensitiveTickImg() const
{
    D_DC(DComboBox);

    return d->insensitiveTickImg;
}

bool DComboBox::isAlert() const
{
    D_DC(DComboBox);

    return d->alert;
}

DComboBoxModel *DComboBox::dcomboBoxModel() const
{
    return qobject_cast<DComboBoxModel*>(QComboBox::model());
}

void DComboBox::setInsensitiveTickImg(const QString &insensitiveTickImg)
{
    D_D(DComboBox);

    d->insensitiveTickImg = insensitiveTickImg;
}

void DComboBox::setAlert(bool alert)
{
    D_D(DComboBox);

    if (d->alert == alert)
        return;

    d->alert = alert;
    emit alertChanged(alert);
}

void DComboBox::setEditable(bool editable)
{
    QComboBox::setEditable(editable);

    view()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    if(lineEdit())
        lineEdit()->setStyleSheet(styleSheet());
}

QString DComboBox::hoverTickImg() const
{
    D_DC(DComboBox);

    return d->hoverTickImg;
}

void DComboBox::setHoverTickImg(const QString &hoverTickImg)
{
    D_D(DComboBox);

    d->hoverTickImg = hoverTickImg;
}

QString DComboBox::normalTickImg() const
{
    D_DC(DComboBox);

    return d->normalTickImg;
}

void DComboBox::setNormalTickImg(const QString &normalTickImg)
{
    D_D(DComboBox);

    d->normalTickImg = normalTickImg;
}

DComboBox::DComboBox(DComboBoxPrivate &dd, QWidget *parent) :
    QComboBox(parent),
    DObject(dd)
{
    D_THEME_INIT_WIDGET(DComboBox, alert);

    d_func()->init();
}

void DComboBox::resizeEvent(QResizeEvent *e)
{
    QComboBox::resizeEvent(e);

    if(!isEditable())
        d_func()->_q_slotCurrentIndexChange(currentIndex());
}

void DComboBox::focusInEvent(QFocusEvent *e)
{
    QComboBox::focusInEvent(e);

    emit focusChanged(true);
}

void DComboBox::focusOutEvent(QFocusEvent *e)
{
    QComboBox::focusOutEvent(e);

    emit focusChanged(false);
}

DWIDGET_END_NAMESPACE

#include "moc_dcombobox.cpp"
