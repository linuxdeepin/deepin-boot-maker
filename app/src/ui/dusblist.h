/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DUSBLIST_H
#define DUSBLIST_H

#include "dlistview.h"
#include <QStringList>
class QStandardItemModel;

class DUsbList : public DListView
{
    Q_OBJECT
public:
    explicit DUsbList(QWidget *parent = 0);

signals:
    void selectDev(const QString& dev);
    void itemClick(const QString& dev);

public slots:
    void selectDriver(QModelIndex);
    void refreshDriverList(const QStringList&);

protected:
    void initList();

    QString             m_DevList;
    QString             m_SelectedDev;
    QModelIndex         m_LastIndex;
    QStandardItemModel *m_StandardItemModel;

    static QString      s_EmptyString;
};

#endif // DUSBLIST_H
