#ifndef UNMOUNTUSBVIEW_H
#define UNMOUNTUSBVIEW_H

#include <DWidget>
#include <QPointer>
#include <DLabel>
#include <DWaterProgress>
#include <DTipLabel>
#include <QVBoxLayout>
#include <DPalette>
#include <DApplicationHelper>
#include <QBrush>
#include <DFontSizeManager>
#include "widgetutil.h"
DWIDGET_USE_NAMESPACE
class UnmountUsbView : public DWidget
{
    Q_OBJECT
public:
    explicit UnmountUsbView(QWidget *parent = nullptr);
private:
    QPointer<DLabel>m_hitTitle;
    QPointer<DLabel>m_ejectIcon;
signals:
     void finish1(quint32 error, const QString &title, const QString &description);
public slots:
};

#endif // UNMOUNTUSBVIEW_H
