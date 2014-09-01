#include "dusblist.h"

#include <QStandardItemModel>

QString DUSBList::s_EmptyString = tr("NO Usb Driver");

DUSBList::DUSBList(QWidget *parent) :
    DListView(parent)
{
    m_standardItemModel = new QStandardItemModel();
    initList();
    this->setFixedSize(160,128);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(selectDriver(QModelIndex)));
}

void DUSBList::initList(){
    m_standardItemModel->clear();
    QStandardItem *standItem = new QStandardItem(s_EmptyString);
    standItem->setIcon(QIcon(":/ui/images/unselect.png"));
    m_standardItemModel->appendRow(standItem);
    this->setModel(m_standardItemModel);
}

void DUSBList::selectDriver(QModelIndex index) {
    QStandardItem *selectItem = m_standardItemModel->itemFromIndex(m_lastIndex);
    if (selectItem){
        selectItem->setIcon(QIcon(":/ui/images/unselect.png"));
    }

    selectItem = m_standardItemModel->itemFromIndex(index);
    if (selectItem){
        selectItem->setIcon(QIcon(":/ui/images/unselect.png"));
    }
    QString dev = index.data(Qt::DisplayRole).toString();
    if (!dev.isEmpty() && (dev != s_EmptyString)) {
        m_selectedDev = dev;
        selectItem->setIcon(QIcon(":/ui/images/select.png"));
        emit selectDev(dev);
    }
    emit itemClick(dev);
}

void DUSBList::refreshDriverList(const QStringList & list) {
    if (list.isEmpty()) {
        initList();
        return;
    }

    m_standardItemModel->clear();
    int selectIndex = 0;
    int rowIndex = 0;

    foreach(QString dev, list) {
        QStandardItem *standItem = new QStandardItem(dev);
        if (dev == m_selectedDev) {
            selectIndex = rowIndex;
        }
        standItem->setIcon(QIcon(":/ui/images/unselect.png"));
        rowIndex++;
        m_standardItemModel->appendRow(standItem);
    }
    QModelIndex selectModelIndex = m_standardItemModel->index(selectIndex,0);
    QStandardItem *selectItem = m_standardItemModel->itemFromIndex(selectModelIndex);
    if (selectItem){
        selectItem->setIcon(QIcon(":/ui/images/select.png"));
        this->setCurrentIndex(selectModelIndex);
        m_lastIndex = selectModelIndex;
    }
    this->setModel(m_standardItemModel);

}
