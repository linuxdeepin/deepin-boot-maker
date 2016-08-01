/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dbuttongrid.h"
#include "dthememanager.h"

#include <QButtonGroup>
#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QLabel>
#include <QDebug>

DWIDGET_BEGIN_NAMESPACE

ImageButton::ImageButton(const QString &icon, const QString text,
                         bool isNameVisible, QWidget *parent):
    QPushButton(parent),
    m_icon(icon),
    m_text(text),
    m_isNameVisible(isNameVisible){
    setProperty("type", "ImageButton");
    initUI();
    initConnect();
}

void ImageButton::initUI(){
    m_iconLabel = new QLabel;
    m_iconLabel->setObjectName("IconLabel");
    m_iconLabel->setProperty("state", "normal");
    m_iconLabel->setPixmap(QPixmap(m_icon));

    QVBoxLayout* mainlayout = new QVBoxLayout;
    mainlayout->addWidget(m_iconLabel, 0, Qt::AlignCenter);

    if (m_isNameVisible){
        m_textLabel = new QLabel;
        m_textLabel->setObjectName("TextLabel");
        m_textLabel->setAlignment(Qt::AlignCenter);
        m_textLabel->setProperty("state", "normal");
        m_textLabel->setText(m_text);
        mainlayout->addWidget(m_textLabel, 0, Qt::AlignCenter);
    }
    mainlayout->setSpacing(0);
    mainlayout->setContentsMargins(5, 5, 5, 5);
    setLayout(mainlayout);

    m_cloesButton = new DImageButton(":/images/dark/images/dock_preview_close_normal.png",
                                     ":/images/dark/images/dock_preview_close_hover.png",
                                     ":/images/dark/images/dock_preview_close_press.png",
                                     ":/images/dark/images/dock_preview_close_press.png",
                                     this);
    m_cloesButton->setFixedSize(20, 20);
    m_cloesButton->setCheckable(false);
    m_cloesButton->hide();
}

void ImageButton::initConnect(){
    connect(this, SIGNAL(toggled(bool)), this, SLOT(handleChecked(bool)));
    connect(m_cloesButton, SIGNAL(clicked()), this, SLOT(handleClose()));
}

QString ImageButton::getId(){
    return m_id;
}

void ImageButton::setId(QString id){
    m_id = id;
}

bool ImageButton::isDeletable(){
    return m_isDeletable;
}

void ImageButton::setDeletable(bool flag){
    m_isDeletable = flag;
}

void ImageButton::handleClose(){
    emit closed(m_icon);
}

void ImageButton::showCloseButton(){
    if (m_isDeletable){
        m_cloesButton->show();
    }else{
        m_cloesButton->hide();
    }
}

void ImageButton::hideCloseButton(){
    m_cloesButton->hide();
}

void ImageButton::handleChecked(bool checked){
    if (checked){
        m_iconLabel->setProperty("state","checked");
        if (m_textLabel){
            m_textLabel->setProperty("state","checked");
        }
        hideCloseButton();
    }else{
        m_iconLabel->setProperty("state","normal");
        if (m_textLabel){
            m_textLabel->setProperty("state","normal");
        }
        showCloseButton();
    }
    updateChildWidgets();
}

void ImageButton::handleHover(bool hovered){
    Q_UNUSED(hovered);
}

void ImageButton::updateChildWidgets(){
    m_iconLabel->style()->unpolish(m_iconLabel);
    m_iconLabel->style()->polish(m_iconLabel);
    m_iconLabel->update();
    if (m_textLabel){
        m_textLabel->style()->unpolish(m_textLabel);
        m_textLabel->style()->polish(m_textLabel);
        m_textLabel->update();
    }
}

void ImageButton::enterEvent(QEvent *event){
    if (m_iconLabel->property("state").isValid()){
        if (m_iconLabel->property("state").toString() == "checked"){

        }else{
             m_iconLabel->setProperty("state","hover");
        }
    }
    if (m_textLabel){
        if (m_textLabel->property("state").isValid()){
            if (m_textLabel->property("state").toString() == "checked"){

            }else{
                 m_textLabel->setProperty("state","hover");
            }
        }
    }
    updateChildWidgets();
    QPushButton::enterEvent(event);
    emit mouseEnter();
}
void ImageButton::leaveEvent(QEvent *event){
    if (m_iconLabel->property("state").isValid()){
        if (m_iconLabel->property("state").toString() == "checked"){

        }else{
             m_iconLabel->setProperty("state","normal");
        }
    }
    if (m_textLabel){
        if (m_textLabel->property("state").isValid()){
            if (m_textLabel->property("state").toString() == "checked"){

            }else{
                 m_textLabel->setProperty("state","normal");
            }
        }
    }
    updateChildWidgets();
    QPushButton::leaveEvent(event);
    emit mouseLeave();
}

void ImageButton::resizeEvent(QResizeEvent *event){
    m_iconLabel->setPixmap(QPixmap(m_icon));
    if (m_textLabel){
        QFontMetrics fm = m_textLabel->fontMetrics();
        m_textLabel->setFixedHeight(fm.height() + 10);
    }
    m_cloesButton->move(m_iconLabel->width() - 10, 0);
    QPushButton::resizeEvent(event);
}

ImageButton::~ImageButton(){

}
ItemButton::ItemButton(QString text,QWidget *parent)
    : QPushButton(parent)
{
    m_text = text;
    this->setText(text);
}
ItemButton::~ItemButton()
{}

void ItemButton::enterEvent(QEvent* event) {
    Q_UNUSED(event);
    emit mouseEntered(m_text);
}
void ItemButton::leaveEvent(QEvent* event) {
    Q_UNUSED(event);
    emit mouseLeaved(m_text);
}
DButtonGrid::DButtonGrid(QWidget *parent) : QTableWidget(parent)
{
    init();
}

DButtonGrid::DButtonGrid(int rows, int columns, QWidget *parent):
    QTableWidget(rows, columns, parent){
    init();
}

void DButtonGrid::init(){
    D_THEME_INIT_WIDGET(DButtonGrid);
    setAttribute(Qt::WA_TranslucentBackground);
    horizontalHeader()->hide();
    verticalHeader()->hide();
    setEditTriggers(NoEditTriggers);
    setDragDropMode(NoDragDrop);
    setSelectionMode(NoSelection);
    setAcceptDrops(false);
    setDragEnabled(false);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setShowGrid(false);

    m_buttonGroup = new QButtonGroup(this);
    m_buttonGroup->setExclusive(true);
    initConnect();
}

void DButtonGrid::initConnect(){
    connect(m_buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(setButtonChecked(int)));
    connect(this, SIGNAL(buttonMouseEntered(QString)), this, SLOT(getButtonEnteredIndex(QString)));
    connect(this, SIGNAL(buttonMouseLeaved(QString)), this, SLOT(getButtonLeavedIndex(QString)));
}

void DButtonGrid::setItemSize(int width, int height){
    m_columnWidth = width;
    m_rowHeight = height;
    for(int i = 0; i< rowCount(); i++){
        setRowHeight(i, height);
    }
    for (int j=0; j < columnCount(); j++){
        setColumnWidth(j, width + 10);
    }

    for(int i= 0; i< m_buttonGroup->buttons().length(); i++){
        if (m_buttonGroup->buttons().at(i)->property("type").toString() == "ImageButton"){
            m_buttonGroup->buttons().at(i)->setFixedSize(width, height);
        }
    }

    setFixedSize((width + 10) * columnCount() + 5, height* rowCount() + 5);
}

void DButtonGrid::addButtonWidget(QPushButton *button, int index){
    int _columnCount = columnCount();
    int row = index / _columnCount;
    int column = index % _columnCount;

    for (int i = rowCount(); i<= row; i++){
        insertRow(i);
    }
    m_buttonGroup->addButton(button, index);

    QFrame* buttonFrame = new QFrame(this);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(button, 0, Qt::AlignCenter);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    buttonFrame->setLayout(mainLayout);
    setCellWidget(row, column, buttonFrame);
    setItemSize(m_columnWidth, m_rowHeight);
}

void DButtonGrid::addButton(const QString &label, int index){
    QFontMetrics fm = fontMetrics();
    int width = fm.width(label);
    QPushButton* button = new ItemButton(label, this);

    button->setFixedWidth(width + 20);
    button->setFixedHeight(fm.height() + 10);
    button->setCheckable(true);
    connect(button, SIGNAL(mouseEntered(QString)), this, SIGNAL(buttonMouseEntered(QString)));
    connect(button, SIGNAL(mouseLeaved(QString)), this, SIGNAL(buttonMouseLeaved(QString)));

    addButtonWidget(button, index);

}

void DButtonGrid::addButtons(const QStringList &listLabels){
    int length = m_buttonLabels.length();
    m_buttonLabels.append(listLabels);
    foreach (QString label, listLabels) {
        int index = listLabels.indexOf(label);
        addButton(label, length + index);
    }
    setItemUnChecked();
}

void DButtonGrid::addImageButton(const QMap<QString, QString> &imageInfo,
                                 int index, bool isNameVisible){

    ImageButton* button = new ImageButton(imageInfo.value("url"), imageInfo.value("name"), isNameVisible);
    if (imageInfo.contains("key")){
        button->setProperty("key", imageInfo.value("key"));
        button->setId(imageInfo.value("key"));
    }

    if (imageInfo.contains("deletable")){
        if (imageInfo.value("deletable") == "true"){
            button->setDeletable(true);
            button->showCloseButton();
        }else{
            button->setDeletable(false);
            button->hideCloseButton();
        }
    }

    button->setCheckable(true);
    addButtonWidget(button, index);

    connect(button, SIGNAL(closed(QString)),this, SLOT(handleClosed(QString)));
}

void DButtonGrid::addImageButtons(const QList<QMap<QString, QString> > &imageInfos, bool isNameVisible){
    int length = m_imageButtonInfos.length();
    m_imageButtonInfos.append(imageInfos);

    for (int i=0; i < imageInfos.count(); i++) {
        addImageButton(imageInfos.at(i), length + i, isNameVisible);
    }
    setItemUnChecked();
    setItemSize(m_columnWidth, m_rowHeight);
}

int DButtonGrid::getCurrentCheckedIndex() const{
    return m_currentCheckedIndex;
}

void DButtonGrid::setButtonChecked(int id){
   QPushButton* button = reinterpret_cast<QPushButton*>(m_buttonGroup->button(id));
   emit buttonCheckedIndexChanged(id);
   m_currentCheckedIndex = id;
   if (button->property("type").isValid() && button->property("type").toString() == "ImageButton"){
       if (button->property("key").isValid()){
           emit buttonChecked(button->property("key").toString());
       }
   }else{
       emit buttonChecked(button->text());
   }
}

int DButtonGrid::getButtonEnteredIndex(QString text) {
    int tmpIndex = m_buttonLabels.indexOf(text);
    emit buttonEnteredIndexChanged(tmpIndex);
    return tmpIndex;
}

int DButtonGrid::getButtonLeavedIndex(QString text) {
    int tmpIndex = m_buttonLabels.indexOf(text);
    emit buttonLeavedIndexChanged(tmpIndex);
    return tmpIndex;
}

void DButtonGrid::checkButtonByIndex(int index){
    if(index == m_currentCheckedIndex)
        return;

    QPushButton* button = reinterpret_cast<QPushButton*>(m_buttonGroup->button(index));
    if(button)
        button->click();
}

void DButtonGrid::checkButtonByText(const QString &label)
{
    for (int i = 0; i < m_buttonGroup->buttons().count(); ++i) {
        QAbstractButton *button = m_buttonGroup->buttons()[i];
        if(button->text() == label)
            checkButtonByIndex(i);
    }
}

void DButtonGrid::setItemUnChecked(){
    for(int i = 0; i< rowCount(); i++){
        for (int j=0; j < columnCount(); j++){
            if (item(i, j)){
                item(i, j)->setFlags(Qt::NoItemFlags);
            }else{
                QTableWidgetItem* item = new QTableWidgetItem();
                item->setFlags(Qt::NoItemFlags);
                setItem(i, j, item);
            }
        }
    }
}

void DButtonGrid::clearData(){
    foreach (QAbstractButton* button, m_buttonGroup->buttons()) {
        m_buttonGroup->removeButton(button);
    }
    m_buttonLabels.clear();
    m_imageButtonInfos.clear();
    m_currentCheckedIndex = -1;
}

void DButtonGrid::clear()
{
    clearData();
    QTableWidget::clear();
    int row = rowCount();
    for (int i=0; i < row; i++){
        removeRow(0);
    }
    setFixedSize(0, 0);
}

void DButtonGrid::handleClosed(QString url){
    Q_UNUSED(url)
    emit requestRefreshed(static_cast<ImageButton*>(sender())->getId());
}

DButtonGrid::~DButtonGrid()
{

}

DWIDGET_END_NAMESPACE
