#include "dlistview.h"

DListView::DListView(QWidget *parent) :
    QListView(parent)
{
    QString qss = "QListView {"
        "font-size: 12px;"
        "color:#b4b4b4;"
        "border: 1px solid gray;"
        "border-radius: 4px;"
        "background: #363636;"
    "}"
    "QListView::item {"
    "    height: 30px;"
    "    border-top: 1px solid #5f5f5f;"
    "    border-bottom: 1px solid #202020;"
    "}"
    "QListView::item:selected {"
        "color: #00bdfd;"
    "}"
    "QListView::item:hover {"
        "color: #ebab4c;"
    "}";
    this->setStyleSheet(qss);

    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setFocusPolicy(Qt::NoFocus);
}
