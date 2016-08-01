/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef BUTTONLISTTAB_H
#define BUTTONLISTTAB_H

#include <QFrame>
class QListWidgetItem;

class ButtonListTab : public QFrame
{
    Q_OBJECT
public:
    explicit ButtonListTab(QWidget *parent = 0);
    ~ButtonListTab();

    void initData();
    void initUI();
    void initConnect();

public slots:
    void handleEnter(QString text);
    void handleLeave(QString text);
private:
    QStringList m_buttons;
};

#endif // BUTTONLISTTAB_H
