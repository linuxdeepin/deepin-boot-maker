/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef BUTTONGRIDTAB_H
#define BUTTONGRIDTAB_H

#include <QFrame>

class ButtonGridTab : public QFrame
{
    Q_OBJECT
public:
    explicit ButtonGridTab(QWidget *parent = 0);
    ~ButtonGridTab();
    void initData();
    void initUI();
    void initConnect();
signals:

public slots:
    void handleClicked(QString label);
    void handleEnterEvent(QString label);
    void handleLeaveEvent(QString label);

    void handleEnterIndex(int index);
    void handleLeaveIndex(int index);
private:
    QStringList m_buttons;
};

#endif // BUTTONGRIDTAB_H
