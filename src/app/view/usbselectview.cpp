#include "usbselectview.h"

#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QCheckBox>

#include "suggestbutton.h"
#include "widgetutil.h"

DWIDGET_USE_NAMESPACE

UsbSelectView::UsbSelectView(QWidget *parent) : QFrame(parent)
{
    setObjectName("UsbSelectView");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 9, 0, 0);

    QLabel *m_title = new QLabel(tr("请选择U盘"));
    m_title->setFixedHeight(38);
    m_title->setStyleSheet("font-size: 26px;");

    QLabel *isoIcon = new QLabel;
    isoIcon->setPixmap(QPixmap(":/media-optical-96px.png"));

    QFrame *usbDeviceListPanel = new QFrame;
    usbDeviceListPanel->setObjectName("UsbDeviceListPanel");
    usbDeviceListPanel->setFixedSize(410, 320);

    QVBoxLayout *usbPanelLayout = new QVBoxLayout(usbDeviceListPanel);
    usbPanelLayout->setContentsMargins(15,0,15,0);

    QCheckBox *m_formatDiskCheck = new QCheckBox;
    m_formatDiskCheck->setObjectName("UsbFormatCheckBox");
    m_formatDiskCheck->setFixedHeight(34);
    m_formatDiskCheck->setText(tr("格式化U盘可提高制作成功率"));
    m_formatDiskCheck->setStyleSheet(WidgetUtil::getQss(":/theme/light/UCheckBox.theme"));
//    usbPanelLayout->addSpacing(62);
//    usbPanelLayout->addWidget(isoIcon, 0, Qt::AlignCenter);
//    usbPanelLayout->addSpacing(5);
//    usbPanelLayout->addWidget(m_fileLabel, 0, Qt::AlignCenter);
//    usbPanelLayout->addSpacing(7);
//    usbPanelLayout->addWidget(m_hits, 0, Qt::AlignCenter);
//    usbPanelLayout->addSpacing(10);
//    usbPanelLayout->addWidget(spliter, 0, Qt::AlignCenter);
//    usbPanelLayout->addSpacing(12);
    usbPanelLayout->addStretch();
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
