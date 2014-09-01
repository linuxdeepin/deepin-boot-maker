#ifndef DUSBLIST_H
#define DUSBLIST_H

#include "dlistview.h"
#include <QStringList>
class QStandardItemModel;

class DUSBList : public DListView
{
    Q_OBJECT
public:
    explicit DUSBList(QWidget *parent = 0);

signals:
    void selectDev(const QString& dev);
    void itemClick(const QString& dev);

public slots:
    void selectDriver(QModelIndex);
    void refreshDriverList(const QStringList&);

protected:
    void initList();

    QString m_devList;
    QString m_selectedDev;
    QStandardItemModel *m_standardItemModel;
    QModelIndex m_lastIndex;

    static QString s_EmptyString;
};

#endif // DUSBLIST_H
