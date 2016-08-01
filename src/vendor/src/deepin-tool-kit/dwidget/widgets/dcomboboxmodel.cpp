/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dcomboboxmodel.h"

DWIDGET_USE_NAMESPACE

DComboBoxModel::DComboBoxModel(QObject *parent) : QAbstractItemModel(parent)
{

}

QModelIndex DComboBoxModel::index(int row, int, const QModelIndex &) const
{
    return this->createIndex(row,0);
}

int DComboBoxModel::count()
{
    return this->rowCount(QModelIndex());
}

void DComboBoxModel::append(const QJsonObject &dataObj)
{
    insert(count(), dataObj);
}

void DComboBoxModel::clear()
{
    this->removeRows(0, count());
}

QJsonObject DComboBoxModel::getJsonData(int index)
{
    QModelIndex tmpIndex = getModelIndex(index);
    QJsonObject tmpObj = this->data(tmpIndex,0).toJsonValue().toObject();

    return tmpObj;
}

bool DComboBoxModel::insert(int index, const QJsonObject &dataObj)
{
    if (insertRow(index))
    {
        if (setData(getModelIndex(index),QVariant(dataObj)))
            return true;
        else
            qWarning() << "setData error";
    }
    qWarning() << "insertRow error" ;
    return false;
}

void DComboBoxModel::remove(int index, int count)
{
    this->removeRows(index,count);
}

void DComboBoxModel::set(int index, const QJsonObject &dataObj)
{
    Q_UNUSED(index)
    Q_UNUSED(dataObj)
}

void DComboBoxModel::setProperty(int index, const QString &property, const QVariant &value)
{
    Q_UNUSED(index)
    Q_UNUSED(property)
    Q_UNUSED(value)
}

QModelIndex DComboBoxModel::getModelIndex(int row) const
{
    return this->index(row,0,QModelIndex());
}

bool DComboBoxModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        m_dataArray.replace(index.row(),QJsonValue(value.toJsonObject()));
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

QModelIndex DComboBoxModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)

    return QModelIndex();
}

int DComboBoxModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_dataArray.count();
}

int DComboBoxModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 1;
}

QVariant DComboBoxModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= m_dataArray.count())
        return QVariant();
    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return QVariant(m_dataArray.at(index.row()));
    else
        return QVariant();
}

bool DComboBoxModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent)

    beginInsertRows(QModelIndex(), row, row + count-1);
    for (int i = row; i < row + count; i++)
    {
        m_dataArray.insert(i,QJsonValue());
    }
    endInsertRows();

    return true;
}

