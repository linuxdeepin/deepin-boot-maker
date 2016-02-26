/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dusblist.h"

#include "../utils.h"
#include <QDebug>
#include <QStandardItemModel>

QString DUsbList::s_EmptyString;

DUsbList::DUsbList(QWidget *parent) :
    DListView(parent)
{
    s_EmptyString = tr("USB drive not detected");
    m_StandardItemModel = new QStandardItemModel();
    initList();
    this->setFixedSize(170,120);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(selectDriver(QModelIndex)));
}

void DUsbList::initList(){
    m_SelectedDev = "";
    m_StandardItemModel->clear();
    QStandardItem *standItem = new QStandardItem(s_EmptyString);
   // standItem->setIcon(QIcon(":/ui/images/unselect.png"));
    standItem->setTextAlignment(Qt::AlignCenter);
    m_StandardItemModel->appendRow(standItem);
    m_LastIndex = m_StandardItemModel->index(0,0);
    this->setModel(m_StandardItemModel);
}

void DUsbList::selectDriver(QModelIndex index) {
    QStandardItem *selectItem = m_StandardItemModel->item(m_LastIndex.row(), m_LastIndex.column());
    m_LastIndex = index;
    if (selectItem){
        selectItem->setIcon(QIcon(":/ui/images/unselect.png"));
    }

    selectItem = m_StandardItemModel->itemFromIndex(index);
    if (selectItem){
        selectItem->setIcon(QIcon(":/ui/images/unselect.png"));
    }
    QString dev = selectItem->data().toString();
    if (!dev.isEmpty() && (dev != s_EmptyString)) {
        m_SelectedDev = dev;
        selectItem->setIcon(QIcon(":/ui/images/select.png"));
        emit selectDev(selectItem->data().toString());
    }
    emit itemClick(selectItem->data().toString());
}

void DUsbList::refreshDriverList(const QStringList & list) {
    if (list.isEmpty()) {
        initList();
        emit selectDev("");
        return;
    }

    m_StandardItemModel->clear();
    int selectIndex = -1;
    int rowIndex = 0;

    foreach(QString dev, list) {
        QStandardItem *standItem = new QStandardItem(Utils::UsbShowText(dev));
        if (dev == m_SelectedDev) {
            selectIndex = rowIndex;
        }
        standItem->setIcon(QIcon(":/ui/images/unselect.png"));
        standItem->setData(QVariant(dev));
        rowIndex++;
        m_StandardItemModel->appendRow(standItem);
    }
    if (-1 == selectIndex) {
        emit selectDev("");
        return;
    }

    QModelIndex selectModelIndex = m_StandardItemModel->index(selectIndex,0);
    QStandardItem *selectItem = m_StandardItemModel->itemFromIndex(selectModelIndex);
    if (selectItem){
        selectItem->setIcon(QIcon(":/ui/images/select.png"));
        this->setCurrentIndex(selectModelIndex);
        m_LastIndex = selectModelIndex;
    }
    this->setModel(m_StandardItemModel);
}
