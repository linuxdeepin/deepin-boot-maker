#include "dusblist.h"

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
    if (selectItem){
        selectItem->setIcon(QIcon(":/ui/images/unselect.png"));
    }

    selectItem = m_StandardItemModel->itemFromIndex(index);
    if (selectItem){
        selectItem->setIcon(QIcon(":/ui/images/unselect.png"));
    }
    QString dev = index.data(Qt::DisplayRole).toString();
    if (!dev.isEmpty() && (dev != s_EmptyString)) {
        m_SelectedDev = dev;
        selectItem->setIcon(QIcon(":/ui/images/select.png"));
        emit selectDev(dev);
    }
    emit itemClick(dev);
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
        QStandardItem *standItem = new QStandardItem(dev);
        if (dev == m_SelectedDev) {
            selectIndex = rowIndex;
        }
        standItem->setIcon(QIcon(":/ui/images/unselect.png"));
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
