#ifndef DEVICEINFOITEM_H
#define DEVICEINFOITEM_H

#include <QWidget>

class QLabel;

class DeviceInfoItem : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceInfoItem(QWidget *parent = 0);

    void setCheck(bool flag);

signals:

public slots:

private:
    bool checked            = false;
    QLabel *m_deviceIcon    = nullptr;


    const QPixmap s_removeDevice    = QPixmap(":/theme/light/image/drive-removable-media-48px.png");
    const QPixmap s_harddiskDevice  = QPixmap(":/theme/light/image/drive-harddisk-48px.png");
    const QPixmap s_selectDevice    = QPixmap(":/theme/light/image/drive-select.png");

};

#endif // DEVICEINFOITEM_H
