/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dcolorcombobox.h"
#include "private/dcombobox_p.h"

DWIDGET_BEGIN_NAMESPACE

class ColorDelegateItem : public DComboBoxItem
{
    Q_OBJECT

public:
    explicit ColorDelegateItem(QWidget *parent = 0);
    void setData(const QString &color, const QString &title);
    void setData(const QVariantMap &map) Q_DECL_OVERRIDE;
    QVariantMap data() const Q_DECL_OVERRIDE;

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
    QColor m_color;

    const int COLOR_BLOCK_WIDTH = 25;
    const int COLOR_BLOCK_HEIGHT = 10;
};

class DComboBoxColorDelegate : public DAbstractComboBoxDelegate
{
public:
    explicit DComboBoxColorDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
};

class DColorComboBoxPrivate : public DComboBoxPrivate
{
    DColorComboBoxPrivate(DColorComboBox *qq);

    D_DECLARE_PUBLIC(DColorComboBox)
};

DColorComboBoxPrivate::DColorComboBoxPrivate(DColorComboBox *qq) :
    DComboBoxPrivate(qq)
{

}

ColorDelegateItem::ColorDelegateItem(QWidget *parent) :
    DComboBoxItem(parent)
{

}

void ColorDelegateItem::setData(const QString &color, const QString &title)
{
    m_color.setNamedColor(color);

    setText(title);

    update();
}

void ColorDelegateItem::setData(const QVariantMap &map)
{
    setData(map["color"].toString(), map["title"].toString());
}

QVariantMap ColorDelegateItem::data() const
{
    QVariantMap map;

    map["color"] = m_color.name();
    map["title"] = text();

    return map;
}

void ColorDelegateItem::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect colorRect(MENU_ITEM_LEFT_MARGIN, height() / 2 - COLOR_BLOCK_HEIGHT / 2,
                    COLOR_BLOCK_WIDTH, COLOR_BLOCK_HEIGHT);
    QBrush b(m_color);
    painter.fillRect(colorRect, b);    //draw header color

    QRect textRect(colorRect.x() + colorRect.width() + TEXT_LEFT_MARGIN, 0, width(), height());

    painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text());  //draw title
}

DComboBoxColorDelegate::DComboBoxColorDelegate(QObject *parent) : DAbstractComboBoxDelegate(parent)
{

}

QWidget * DComboBoxColorDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    ColorDelegateItem * editor = new ColorDelegateItem(parent);
    editor->setFixedHeight(MENU_ITEM_HEIGHT);

    return editor;
}

void DComboBoxColorDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QJsonObject dataObj = index.model()->data(index, Qt::DisplayRole).toJsonValue().toObject();

    if (dataObj.isEmpty())
        return;

    ColorDelegateItem *colorItem = static_cast<ColorDelegateItem*>(editor);
    QString color = "#FFFFFFFF";
    QString title = "";
    if (dataObj.contains("itemTitle"))
        title = dataObj.value("itemTitle").toString();
    if (dataObj.contains("itemColor"))
        color = dataObj.value("itemColor").toString();

    if (colorItem)
        colorItem->setData(color, title);
}

DColorComboBox::DColorComboBox(QWidget *parent) :
    DComboBox(*new DColorComboBoxPrivate(this), parent)
{
    DComboBoxColorDelegate *d = new DComboBoxColorDelegate(this);
    setItemDelegate(d);

    setModel(new DComboBoxModel(this));
    d_func()->setMaskLabel(new ColorDelegateItem(this));

    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentIndexChange(int)));
}

void DColorComboBox::addData(const QColor &color, const QString &title)
{
    QJsonObject colorObj;
    colorObj.insert("itemTitle", QJsonValue(title));
    colorObj.insert("itemColor", color.name(QColor::HexArgb));

    dcomboBoxModel()->append(colorObj);
    // Make the combo boxes always displayed.
    view()->openPersistentEditor(dcomboBoxModel()->getModelIndex(dcomboBoxModel()->count() - 1));
}

void DColorComboBox::addData(const QString &color, const QString &title)
{
    QJsonObject colorObj;
    colorObj.insert("itemTitle", QJsonValue(title));
    colorObj.insert("itemColor", color);

    dcomboBoxModel()->append(colorObj);
    // Make the combo boxes always displayed.
    view()->openPersistentEditor(dcomboBoxModel()->getModelIndex(dcomboBoxModel()->count() - 1));
}

void DColorComboBox::onCurrentIndexChange(int index)
{
    QJsonObject colorObj = dcomboBoxModel()->getJsonData(index);

    emit currentColorChange(QColor(colorObj["itemColor"].toString()));
}

DWIDGET_END_NAMESPACE

#include "dcolorcombobox.moc"
