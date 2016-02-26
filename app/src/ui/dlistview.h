/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DLISTVIEW_H
#define DLISTVIEW_H

#include <QListView>

class DListView : public QListView
{
    Q_OBJECT
public:
    explicit DListView(QWidget *parent = 0);

signals:

public slots:

};

#endif // DLISTVIEW_H
