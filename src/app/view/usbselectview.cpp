#include "usbselectview.h"

#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QListWidget>

#include "suggestbutton.h"
#include "widgetutil.h"
#include "deviceinfoitem.h"
#include "devicelistwidget.h"
#include "dwaterprogress.h"

DWIDGET_USE_NAMESPACE

UsbSelectView::UsbSelectView(QWidget *parent) : QFrame(parent)
{
    setObjectName("UsbSelectView");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 9, 0, 0);

    QLabel *m_title = new QLabel(tr("Please Select ISO File"));
    m_title->setFixedHeight(38);
    m_title->setStyleSheet("font-size: 26px;");

    QLabel *isoIcon = new QLabel;
    isoIcon->setPixmap(QPixmap(":/media-optical-96px.png"));

    QFrame *usbDeviceListPanel = new QFrame;
    usbDeviceListPanel->setObjectName("UsbDeviceListPanel");
    usbDeviceListPanel->setFixedSize(410, 320);

    QVBoxLayout *usbPanelLayout = new QVBoxLayout(usbDeviceListPanel);
    usbPanelLayout->setContentsMargins(10, 0, 10, 0);

    QCheckBox *m_formatDiskCheck = new QCheckBox;
    m_formatDiskCheck->setObjectName("UsbFormatCheckBox");
    m_formatDiskCheck->setFixedHeight(34);
    m_formatDiskCheck->setText(tr("Format to improve successful"));
    m_formatDiskCheck->setStyleSheet(WidgetUtil::getQss(":/theme/light/UCheckBox.theme"));

    DeviceListWidget *m_deviceList = new DeviceListWidget;
    m_deviceList->setObjectName("UsbDeviceList");
    m_deviceList->setFixedSize(390, 270);

    setFocusPolicy(Qt::NoFocus);

    for (int i = 0; i < 3; ++i) {
        QListWidgetItem *testItem = new QListWidgetItem;
        auto myItem = new DeviceInfoItem;
        testItem->setSizeHint(myItem->size());
        m_deviceList->addItem(testItem);
        m_deviceList->setItemWidget(testItem, myItem);
        myItem->setCheck(true);
    }
    usbPanelLayout->addWidget(m_deviceList, 0, Qt::AlignLeft);
    usbPanelLayout->addSpacing(15);
    usbPanelLayout->addWidget(m_formatDiskCheck, 0, Qt::AlignLeft);

    SuggestButton *start = new SuggestButton();
    start->setObjectName("StartMake");
    start->setText(tr("Start Make"));

    mainLayout->addWidget(m_title, 0, Qt::AlignCenter);
    mainLayout->addSpacing(24);
    mainLayout->addWidget(usbDeviceListPanel, 0, Qt::AlignCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(start, 0, Qt::AlignCenter);

    this->setStyleSheet(WidgetUtil::getQss(":/theme/light/UsbSelectView.theme"));
}
