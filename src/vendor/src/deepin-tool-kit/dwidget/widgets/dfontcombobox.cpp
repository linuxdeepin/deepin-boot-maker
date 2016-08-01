/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QFontMetrics>

#include "dfontcombobox.h"
#include "private/dcombobox_p.h"

DWIDGET_BEGIN_NAMESPACE

class FontDelegateItem : public DComboBoxItem
{
    Q_OBJECT

public:
    explicit FontDelegateItem(QWidget *parent = 0);

    void setItemFont(const QString &family, const QString &title);
};

class DComboBoxFontDelegate : public DAbstractComboBoxDelegate
{
public:
    explicit DComboBoxFontDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
};

FontDelegateItem::FontDelegateItem(QWidget *parent) :
    DComboBoxItem(parent)
{

}

void FontDelegateItem::setItemFont(const QString &family, const QString &title)
{
    QFont font = this->font();

    font.setFamily(family);

    setFont(font);

    setText(title.isEmpty() ? family : title);
}

DComboBoxFontDelegate::DComboBoxFontDelegate(QObject *parent) : DAbstractComboBoxDelegate(parent)
{

}

QWidget * DComboBoxFontDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    FontDelegateItem * editor = new FontDelegateItem(parent);
    editor->setFixedHeight(MENU_ITEM_HEIGHT);

    return editor;
}

void DComboBoxFontDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QJsonObject dataObj = index.model()->data(index, Qt::DisplayRole).toJsonValue().toObject();

    if (dataObj.isEmpty())
        return;

    FontDelegateItem *fontItem = static_cast<FontDelegateItem*>(editor);
    QString title = "";
    QString family = "";
    if (fontItem && dataObj.contains("itemTitle"))
        title = dataObj.value("itemTitle").toString();
    if (fontItem && dataObj.contains("itemFontFamily"))
        family = dataObj.value("itemFontFamily").toString();

    fontItem->setItemFont(family, title);
}

DFontComboBox::DFontComboBox(QWidget *parent) : DComboBox(parent)
{
    DComboBoxFontDelegate *dbfb = new DComboBoxFontDelegate(this);
    setItemDelegate(dbfb);

    setModel(new DComboBoxModel(this));

    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentIndexChange(int)));
}

void DFontComboBox::addFontItem(const QString &family, const QString &title)
{
    QJsonObject nameObj;
    nameObj.insert("itemFontFamily", QJsonValue(family));
    nameObj.insert("itemTitle", QJsonValue(title));

    dcomboBoxModel()->append(nameObj);
    // Make the combo boxes always displayed.
    view()->openPersistentEditor(dcomboBoxModel()->getModelIndex(dcomboBoxModel()->count() - 1));
}

void DFontComboBox::onCurrentIndexChange(int index)
{
    QJsonObject nameObj = dcomboBoxModel()->getJsonData(index);

    emit currentFontNameChange(nameObj["itemFont"].toString());
}

DWIDGET_END_NAMESPACE

#include "dfontcombobox.moc"
