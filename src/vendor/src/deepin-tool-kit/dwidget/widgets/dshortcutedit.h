/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DSHORTCUTEDIT_H
#define DSHORTCUTEDIT_H

#include <QFrame>
#include <QLabel>
#include <QString>
#include <QRegExp>
#include <QColor>
#include <QMap>

#include "dwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class DShortcutEditLabel;
class LIBDTKWIDGETSHARED_EXPORT DShortcutEdit : public QFrame
{
    Q_OBJECT

public:
    DShortcutEdit(QWidget *parent = 0);

    QSize sizeHint() const;
    bool eventFilter(QObject *o, QEvent *e);
    bool isValidShortcutKey(const QString & key);
    const QMap<QString, QString> &getKeyMapping() const;
    const QList<QRegExp> &getBlockShortcutKeysList() const;

signals:
    void shortcutKeysChanged(const QString & shortcutKeys);
    void shortcutKeysFinished(const QString & shortcutKeys);
    void invalidShortcutKey(const QString & shortcutKeys);

public slots:
    void clearShortcutKey();
    void setShortcutKey(const QString & key);
    void setKeyMapping(const QMap<QString, QString> & mapping);
    void setBlockShortcutKeysList(const QList<QRegExp> & kList);
    void setInValidState() const;
    void setNormalState() const;

private slots:
    void toEchoMode();
    void toInputMode() const;
    void shortcutKeyPress(QKeyEvent *e);

private:
    QString convertShortcutKeys(const QString & keys);

private:
    DShortcutEditLabel *m_keysLabel;
    QLabel *m_keysEdit;

    QString m_shortcutKeys;
    QList<QRegExp> m_blockedShortcutKeys;
    QMap<QString, QString> m_keyMapping;

    static const QString DefaultTips;
};

class DShortcutEditLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(QColor echoNormal MEMBER m_colorNormal NOTIFY colorSettingChange DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY(QColor echoHover MEMBER m_colorHover NOTIFY colorSettingChange DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY(QColor echoInvalid MEMBER m_colorInvalid NOTIFY colorSettingChange DESIGNABLE true SCRIPTABLE true)

public:
    enum EchoState {Normal = 1, Hover, Invalid};

public:
    DShortcutEditLabel(QWidget * parent = 0);

    void setEchoState(const EchoState state);

signals:
    void colorSettingChange();

private:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private:
    QColor m_colorNormal;
    QColor m_colorHover;
    QColor m_colorInvalid;

    EchoState m_state = Normal;
};

DWIDGET_END_NAMESPACE

#endif // DSHORTCUTEDIT_H
