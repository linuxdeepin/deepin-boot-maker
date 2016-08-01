/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QRegularExpressionValidator>
#include <QHBoxLayout>
#include <QGuiApplication>
#include <QClipboard>
#include <QKeyEvent>
#include <QDebug>

#include "dthememanager.h"
#include "dipv4lineedit.h"
#include "private/dipv4lineedit_p.h"

DWIDGET_BEGIN_NAMESPACE

#define RX_PATTERN_IP "^(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)?$"

DIpv4LineEditPrivate::DIpv4LineEditPrivate(DIpv4LineEdit *parent) :
    DLineEditPrivate(parent)
{

}

void DIpv4LineEditPrivate::init()
{
    D_Q(DIpv4LineEdit);

    editMainWidget = new QWidget(q);

    QHBoxLayout *hbox_layout = new QHBoxLayout;

    hbox_layout->setSpacing(0);
    hbox_layout->setMargin(0);
    hbox_layout->addWidget(getEdit());
    hbox_layout->addWidget(getLabel());
    hbox_layout->addWidget(getEdit());
    hbox_layout->addWidget(getLabel());
    hbox_layout->addWidget(getEdit());
    hbox_layout->addWidget(getLabel());
    hbox_layout->addWidget(getEdit());

    editMainWidget->setLayout(hbox_layout);

    q->DLineEdit::setReadOnly(true);
    q->setFocusProxy(editList.first());
    q->setValidator(new QRegularExpressionValidator(QRegularExpression("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)?\\.){0,3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)?"), q));

    QObject::connect(q, &DIpv4LineEdit::sizeChanged,
                     editMainWidget, static_cast<void (QWidget::*)(const QSize&)>(&QWidget::resize));
    QObject::connect(q, SIGNAL(textChanged(QString)), q, SLOT(_q_setIpLineEditText(QString)), Qt::QueuedConnection);
}

QLabel *DIpv4LineEditPrivate::getLabel()
{
    QLabel *label = new QLabel(".");

    label->setObjectName("DIpv4LineEdit_Label");
    label->setCursor(Qt::IBeamCursor);

    return label;
}

QLineEdit *DIpv4LineEditPrivate::getEdit()
{
    D_Q(DIpv4LineEdit);

    QLineEdit *edit = new QLineEdit;

    edit->setObjectName("DIpv4LineEdit_Edit");
    edit->setValidator(new QRegularExpressionValidator(QRegularExpression(RX_PATTERN_IP), edit));
    edit->setAlignment(Qt::AlignHCenter);
    edit->setAttribute(Qt::WA_InputMethodEnabled, false);
    edit->setContextMenuPolicy(Qt::NoContextMenu);
    edit->installEventFilter(q);

    editList << edit;

    QObject::connect(edit, SIGNAL(textEdited(QString)), q, SLOT(_q_updateLineEditText()));

    return edit;
}

void DIpv4LineEditPrivate::setFocus(bool focus)
{
    if(this->focus == focus)
        return;

    this->focus = focus;

    emit q_func()->focusChanged(focus);
}

void DIpv4LineEditPrivate::_q_updateLineEditText()
{
    D_Q(DIpv4LineEdit);

    QString text;

    for(const QLineEdit *edit : editList) {
        text.append(".").append(edit->text());
    }

    QObject::disconnect(q, SIGNAL(textChanged(QString)), q, SLOT(_q_setIpLineEditText(QString)));

    if(text == "....") {
        if(!q->text().isEmpty()) {
            q->setText("");
            emit q->textEdited(q->text());
        }
    } else {
        text = text.mid(1);

        if(text != q->text()) {
            q->setText(text);
            emit q->textEdited(q->text());
        }
    }

    QObject::connect(q, SIGNAL(textChanged(QString)), q, SLOT(_q_setIpLineEditText(QString)), Qt::QueuedConnection);

    q->DLineEdit::setCursorPosition(q->cursorPosition());
}

void DIpv4LineEditPrivate::_q_setIpLineEditText(const QString &text)
{
    D_QC(DIpv4LineEdit);

    int pos = 0;

    if(q->validator()->validate(const_cast<QString&>(text), pos) != QValidator::Acceptable) {
        _q_updateLineEditText();
        return;
    }

    const QStringList text_list = text.split('.');

    int min_count = qMin(editList.count(), text_list.count());

    for(int i = 0; i < min_count; ++i ) {
        QLineEdit *edit = editList[i];
        bool edit_blocked = edit->blockSignals(true);
        edit->setText(text_list[i]);
        edit->blockSignals(edit_blocked);
    }

    for(int i = min_count; i < editList.count(); ++i) {
        QLineEdit *edit = editList[i];
        bool edit_blocked = edit->blockSignals(true);
        edit->clear();
        edit->blockSignals(edit_blocked);
    }
}

DIpv4LineEdit::DIpv4LineEdit(QWidget *parent) :
    DLineEdit(*new DIpv4LineEditPrivate(this), parent)
{
    D_THEME_INIT_WIDGET(DIpv4LineEdit);

    d_func()->init();
}

QString DIpv4LineEdit::displayText() const
{
    return text();
}

int DIpv4LineEdit::cursorPosition() const
{
    D_DC(DIpv4LineEdit);

    int cursorPosition = 0;

    for(const QLineEdit *edit : d->editList) {
        if(edit->hasFocus()) {
            cursorPosition += edit->cursorPosition();
            break;
        } else {
            cursorPosition += edit->text().count() + 1;
        }
    }

    return cursorPosition;
}

Qt::Alignment DIpv4LineEdit::alignment() const
{
    return Qt::AlignHCenter;
}

bool DIpv4LineEdit::hasAcceptableInput() const
{
    D_DC(DIpv4LineEdit);

    bool has = true;

    for(const QLineEdit *edit : d->editList) {
        has = has && edit->hasAcceptableInput();
    }

    return has;
}

bool DIpv4LineEdit::isReadOnly() const
{
    return d_func()->editList.first()->isReadOnly();
}

void DIpv4LineEdit::setCursorPosition(int cursorPosition)
{
    DLineEdit::setCursorPosition(cursorPosition);

    D_D(DIpv4LineEdit);

    for(QLineEdit *edit : d->editList) {
        if(cursorPosition > edit->text().count()) {
            cursorPosition -= edit->text().count();
            --cursorPosition;
        } else {
            edit->setCursorPosition(cursorPosition);
            edit->setFocus();

            break;
        }
    }
}

void DIpv4LineEdit::setReadOnly(bool readOnly)
{
    D_D(DIpv4LineEdit);

    for(QLineEdit *edit : d->editList) {
        edit->setReadOnly(readOnly);
    }
}

void DIpv4LineEdit::setSelection(int start, int length)
{
    D_D(DIpv4LineEdit);

    for(QLineEdit *edit : d->editList) {
        if(edit->text().count() > start) {
            if(edit->text().count() < length + start) {
                int tmp_length = edit->text().count() - start;

                edit->setSelection(start, tmp_length);

                length -= tmp_length;
            } else {
                edit->setSelection(start, length);
                break;
            }
        } else {
            edit->setSelection(edit->cursorPosition(), 0);
        }

        start -= edit->text().count();
    }

    DLineEdit::setSelection(start, length);
}

void DIpv4LineEdit::selectAll()
{
    D_D(DIpv4LineEdit);

    for(QLineEdit *edit : d->editList) {
        edit->selectAll();
    }

    DLineEdit::selectAll();
}

bool DIpv4LineEdit::eventFilter(QObject *obj, QEvent *e)
{
    if(e->type() == QEvent::KeyPress) {
        QLineEdit *edit = qobject_cast<QLineEdit*>(obj);

        if(edit) {
            QKeyEvent *event = static_cast<QKeyEvent*>(e);

            if(event) {
                D_D(DIpv4LineEdit);

                if(event->key() <= Qt::Key_9 && event->key() >= Qt::Key_0) {
                    if(edit->cursorPosition() == edit->text().count()) {
                        QRegularExpression rx(RX_PATTERN_IP);

                        const QString number = QString::number(event->key() - Qt::Key_0);

                        if(!rx.match(edit->text().append(number)).hasMatch()) {
                            int index = d->editList.indexOf(edit) + 1;

                            if(index < d->editList.count()) {
                                d->editList[index]->setFocus();

                                if(d->editList[index]->text().isEmpty()) {
                                    d->editList[index]->setText(number);
                                    d->_q_updateLineEditText();
                                }
                            }

                            return true;
                        }
                    }
                } else {
                    if(event->key() == Qt::Key_Backspace) {
                        bool accept = false;

                        for(QLineEdit *edit : d->editList) {
                            if(!edit->selectedText().isEmpty()) {
                                edit->setText(edit->text().remove(edit->selectedText()));
                                d->_q_updateLineEditText();
                                accept = true;
                            }
                        }

                        if(accept)
                            return true;
                    }

                    if(event->key() == Qt::Key_Left
                              || (event->key() == Qt::Key_Backspace
                                  && edit->cursorPosition() == 0)) {
                        setCursorPosition(cursorPosition() - 1);

                        return true;
                    }

                    if(event->key() == Qt::Key_Right) {
                        setCursorPosition(cursorPosition() + 1);

                        return true;
                    }

                    if(event->key() == Qt::Key_Period || event->key() == Qt::Key_Space) {
                        int index = d->editList.indexOf(edit) + 1;

                        if(index < d->editList.count()) {
                            d->editList[index]->setFocus();
                        }

                        return true;
                    }

                    if(event->modifiers() == Qt::ControlModifier) {
                        if(event->key() == Qt::Key_V) {
                            QString clipboarg_text = qApp->clipboard()->text();
                            QString text = edit->text().insert(edit->cursorPosition(), clipboarg_text);

                            QRegularExpression rx(RX_PATTERN_IP);

                            if(rx.match(text).hasMatch()) {
                                edit->setText(text);
                                d->_q_updateLineEditText();
                            } else {
                                int pos = 0;

                                if(this->validator()->validate(clipboarg_text, pos) == QValidator::Acceptable)
                                    d->_q_setIpLineEditText(clipboarg_text);
                            }

                            return true;
                        } else if(event->key() == Qt::Key_A) {
                            selectAll();
                        } else if(event->key() == Qt::Key_X) {
                            cut();
                        } else {
                            DLineEdit::keyPressEvent(event);
                        }

                        return true;
                    }
                }
            }
        }
    } else if(e->type() == QEvent::FocusIn) {
        QLineEdit *edit = qobject_cast<QLineEdit*>(obj);

        if(edit) {
            DLineEdit::setCursorPosition(cursorPosition());
        }

        d_func()->setFocus(true);
    } else if(e->type() == QEvent::FocusOut || e->type() == QEvent::MouseButtonPress) {
        D_D(DIpv4LineEdit);

        bool focus = false;

        for(QLineEdit *edit : d->editList) {
            edit->setSelection(edit->cursorPosition(), 0);

            focus = edit->hasFocus() | focus;
        }

        if(!focus)
            d_func()->setFocus(false);
    }

    return DLineEdit::eventFilter(obj, e);
}

#include "moc_dipv4lineedit.cpp"

DWIDGET_END_NAMESPACE
