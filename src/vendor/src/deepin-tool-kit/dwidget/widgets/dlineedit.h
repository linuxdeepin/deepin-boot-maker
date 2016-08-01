/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DLINEEDIT_H
#define DLINEEDIT_H

#include <QLineEdit>

#include "dwidget_global.h"
#include "dobject.h"

DWIDGET_BEGIN_NAMESPACE

class DLineEditPrivate;
class LIBDTKWIDGETSHARED_EXPORT DLineEdit : public QLineEdit, public DObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DLineEdit)
    D_DECLARE_PRIVATE(DLineEdit)
    Q_PROPERTY(bool alert READ isAlert WRITE setAlert NOTIFY alertChanged)
    Q_PROPERTY(QString normalIcon READ normalIcon WRITE setNormalIcon DESIGNABLE true)
    Q_PROPERTY(QString hoverIcon READ hoverIcon WRITE setHoverIcon DESIGNABLE true)
    Q_PROPERTY(QString pressIcon READ pressIcon WRITE setPressIcon DESIGNABLE true)

public:
    DLineEdit(QWidget *parent = 0);

    void setAlert(bool isAlert);
    bool isAlert() const;

    void setIconVisible(bool visible);
    bool iconVisible() const;

    QString normalIcon() const;
    QString hoverIcon() const;
    QString pressIcon() const;

public Q_SLOTS:
    void setNormalIcon(const QString &normalIcon);
    void setHoverIcon(const QString &hoverIcon);
    void setPressIcon(const QString &pressIcon);

Q_SIGNALS:
    void iconClicked() const;
    void alertChanged(bool alert) const;
    void focusChanged(bool onFocus) const;
    void sizeChanged(const QSize &size) const;

protected:
    DLineEdit(DLineEditPrivate &q, QWidget *parent);

    void focusInEvent(QFocusEvent *e) Q_DECL_OVERRIDE;
    void focusOutEvent(QFocusEvent *e) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;

    Q_PRIVATE_SLOT(d_func(), void _q_resizeInsideFrame(const QSize &))
};

DWIDGET_END_NAMESPACE

#endif // DLINEEDIT_H
