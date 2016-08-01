/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DBUTTONGRID_H
#define DBUTTONGRID_H

#include <QTableWidget>
#include <QPushButton>
#include <QResizeEvent>

#include "dwidget_global.h"
#include "dconstants.h"
#include "dimagebutton.h"

class QButtonGroup;
class QLabel;


DWIDGET_BEGIN_NAMESPACE


class ImageButton: public QPushButton
{
    Q_OBJECT
public:
    ImageButton(const QString& icon, const QString text, bool isNameVisible=true, QWidget *parent = 0);
    ~ImageButton();
    void initUI();
    void initConnect();
    QString getId();
    void updateChildWidgets();
    bool isDeletable();


public slots:
    void setId(QString id);
    void handleChecked(bool checked);
    void handleHover(bool hovered);

    void showCloseButton();
    void hideCloseButton();

    void setDeletable(bool flag);
    void handleClose();

signals:
    void mouseEnter();
    void mouseLeave();
    void closed(QString url);

protected:
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
    void resizeEvent(QResizeEvent* event);

private:
    QString m_icon;
    QString m_text;
    QString m_id;
    bool m_isNameVisible;
    QLabel* m_iconLabel = NULL;
    QLabel* m_textLabel = NULL;
    DImageButton* m_cloesButton;
    bool m_isDeletable = false;
};

class ItemButton: public QPushButton
{
    Q_OBJECT
public:
    ItemButton(QString text, QWidget *parent=0);
    ~ItemButton();
signals:
    void mouseEntered(QString buttonId);
    void mouseLeaved(QString buttonId);
    void mouseEntered(int index);
    void mouseLeaved(int index);
protected:
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
private:
    QString m_text;
};
class LIBDTKWIDGETSHARED_EXPORT DButtonGrid : public QTableWidget
{
    Q_OBJECT
public:
    explicit DButtonGrid(QWidget *parent = 0);
    explicit DButtonGrid(int rows, int columns, QWidget *parent = 0);
    ~DButtonGrid();

    void init();
    void initConnect();
    void setItemUnChecked();
    void setItemSize(int width, int height);
    int getCurrentCheckedIndex() const;

public slots:
    void addButton(const QString& label, int index);
    void addButtons(const QStringList& listLabels);
    void addImageButton(const QMap<QString, QString>& imageInfo, int index, bool isNameVisible=true);
    void addImageButtons(const QList<QMap<QString, QString>>& imageInfos, bool isNameVisible=true);
    void addButtonWidget(QPushButton* button, int index);
    void checkButtonByIndex(int index);
    void checkButtonByText(const QString& label);
    void clear();
    void handleClosed(QString url);

private slots:
    void setButtonChecked(int id);
    int getButtonEnteredIndex(QString text);
    int getButtonLeavedIndex(QString text);
signals:
    void buttonChecked(QString label);
    void buttonCheckedIndexChanged(int index);

    void buttonEnteredIndexChanged(int index);
    void buttonLeavedIndexChanged(int index);

    void buttonMouseEntered(QString label);
    void buttonMouseLeaved(QString label);

    void requestRefreshed(QString url);

private:
    QButtonGroup* m_buttonGroup = NULL;
    QStringList m_buttonLabels;
    QList<QMap<QString, QString>> m_imageButtonInfos;
    int m_rowHeight = 40;
    int m_columnWidth = 100;
    int m_currentCheckedIndex = -1;

    void clearData();
};

DWIDGET_END_NAMESPACE

#endif // DBUTTONGRID_H
