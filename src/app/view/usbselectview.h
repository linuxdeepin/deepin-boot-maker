#ifndef USBSELECTVIEW_H
#define USBSELECTVIEW_H

#include <QFrame>

class UsbSelectView : public QFrame
{
    Q_OBJECT
public:
    explicit UsbSelectView(QWidget *parent = 0);

signals:
    void deviceSelected(const QString &partition, bool format);

public slots:
};

#endif // USBSELECTVIEW_H
