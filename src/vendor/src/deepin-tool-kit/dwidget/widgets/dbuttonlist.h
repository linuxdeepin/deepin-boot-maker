/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DBUTTONLIST_H
#define DBUTTONLIST_H

#include <QListWidget>
#include <QPushButton>
#include "dwidget_global.h"

class QButtonGroup;
class QLabel;
class QPoint;
class QResizeEvent;
class QEvent;

class IconButton : public QPushButton{
    Q_OBJECT
public:
    IconButton(const QString& Icon, const QString& text, QWidget* parent=0);

    void initIconLabel();
    void initConnect();
    void setIconLeftMargin(int leftMargin);
    void setIconLabel(const QString& icon);
    void hideIconLabel();
    void updateStyle();

signals:
    void mouseEntered(QString label);
    void mouseLeaved(QString label);

protected:
    void resizeEvent(QResizeEvent* event);
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);

private:
    QLabel* m_iconLabel;
    QString m_icon;
    QString m_text;
};


DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DButtonList : public QListWidget
{
    Q_OBJECT
public:
    explicit DButtonList(QWidget *parent = 0);
    ~DButtonList();

    void initMargins(int leftMargin, int rightMargin, int imageLeftMargin);
    IconButton* getButtonByIndex(int index);

public slots:
    void addButton(const QString &label);
    void addButton(const QString& label, int index);
    void addButtons(const QStringList& listLabels);
    void setItemHeight(int height);
    void setItemWidth(int width);
    void setItemSize(int width, int height);
    void setItemSize(QSize size);
    void checkButtonByIndex(int index);
    void clear();

private slots:
    void setButtonChecked(int id);

signals:
    void buttonChecked(QString label);
    void buttonCheckedIndexChanged(int index);
    void buttonMouseEntered(QString label);
    void buttonMouseLeaved(QString label);

private:
    QButtonGroup* m_buttonGroup = NULL;

    void initConnect();

    int m_leftMargin = 10;
    int m_rightMargin = 10;
    int m_imageLeftMargin = 10;
};


DWIDGET_END_NAMESPACE

#endif // DBUTTONLIST_H
