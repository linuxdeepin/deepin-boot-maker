#include "usbselectview.h"

#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QListWidget>
#include <QMessageBox>
#include <QIcon>

#include <ddialog.h>

#include "suggestbutton.h"
#include "widgetutil.h"
#include "deviceinfoitem.h"
#include "devicelistwidget.h"
#include "dwaterprogress.h"

#include <bminterface.h>

DWIDGET_USE_NAMESPACE

static QString usageString(quint32 usage, quint32 total)
{
    if (total <= 0 || usage > total) {
        return "0/0G";
    }

    if (total <= 1024) {
        return QString("%1/%2M").arg(usage).arg(total);
    }

    return QString("%1/%2G")
           .arg(QString::number(static_cast<double>(usage) / 1024, 'f', 2))
           .arg(QString::number(static_cast<double>(total) / 1024, 'f', 2));
}


static int percent(quint32 usage, quint32 total)
{
    if (total <= 0) {
        return 0;
    }

    if (usage > total) {
        return 100;
    }

    return static_cast<int>(usage * 100 / total);
}

UsbSelectView::UsbSelectView(QWidget *parent) : QFrame(parent)
{
    setObjectName("UsbSelectView");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 9, 0, 0);

    QLabel *m_title = new QLabel(tr("Please select a disk"));
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
    m_formatDiskCheck->setText(tr("Formating disk can increase the making success rate"));
    m_formatDiskCheck->setStyleSheet(WidgetUtil::getQss(":/theme/light/UCheckBox.theme"));
    m_formatDiskCheck->hide();

    DeviceListWidget *m_deviceList = new DeviceListWidget;
    m_deviceList->setObjectName("UsbDeviceList");
    m_deviceList->setFixedSize(390, 270);
    m_deviceList->hide();

    QLabel *m_warningHint = new  QLabel("");
    m_warningHint->setObjectName("WarningHint");
    m_warningHint->setFixedWidth(390);
    m_warningHint->setWordWrap(true);

    QLabel *m_emptyHint = new  QLabel(tr("No available disk found"));
    m_emptyHint->setObjectName("EmptyHintTitle");

    usbPanelLayout->addStretch();
    usbPanelLayout->addWidget(m_emptyHint, 0, Qt::AlignCenter);
    usbPanelLayout->addStretch();
    usbPanelLayout->addWidget(m_deviceList, 0, Qt::AlignLeft);
    usbPanelLayout->addSpacing(15);
    usbPanelLayout->addWidget(m_formatDiskCheck, 0, Qt::AlignCenter);

    SuggestButton *start = new SuggestButton();
    start->setObjectName("StartMake");
    start->setText(tr("Start making"));
    start->setDisabled(true);

    mainLayout->addWidget(m_title, 0, Qt::AlignCenter);
    mainLayout->addSpacing(24);
    mainLayout->addWidget(usbDeviceListPanel, 0, Qt::AlignCenter);
    mainLayout->addWidget(m_warningHint, 0, Qt::AlignCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(start, 0, Qt::AlignCenter);

    this->setStyleSheet(WidgetUtil::getQss(":/theme/light/UsbSelectView.theme"));

    connect(m_formatDiskCheck, &QCheckBox::clicked, this, [ = ](bool checked) {
        if (!checked) {
            m_warningHint->setText("");
            return;
        }
        m_warningHint->setText(tr("The disk data will be completely  deleted by formating, please confirm and continue"));
        this->adjustSize();
        return;

//        DDialog msgbox(this);
//        msgbox.setFixedWidth(300);
//        msgbox.setIcon(QMessageBox::standardIcon(QMessageBox::Warning));
//        msgbox.setWindowTitle(tr("Format USB flash drive"));
//        msgbox.setTextFormat(Qt::AutoText);
//        msgbox.setMessage(tr("All data will be lost during formatting, please back up in advance and then press OK button."));
//        msgbox.addButtons(QStringList() << tr("Ok") << tr("Cancel"));

//        m_formatDiskCheck->setChecked(0 == msgbox.exec());
    });

    connect(BMInterface::instance(), &BMInterface::deviceListChanged,
    this, [ = ](const QList<DeviceInfo> &partitions) {
        bool hasPartitionSelected = false;
        m_formatDiskCheck->setVisible(partitions.size());
        m_emptyHint->setVisible(!partitions.size());
        m_deviceList->setVisible(partitions.size());
//        m_formatDiskCheck->setEnabled(partitions.size());

        m_deviceList->clear();
        foreach(const DeviceInfo & partition, partitions) {
            QListWidgetItem *listItem = new QListWidgetItem;
            DeviceInfoItem *infoItem = new DeviceInfoItem(
                partition.label,
                partition.path,
                usageString(partition.used, partition.total),
                percent(partition.used, partition.total));
            listItem->setSizeHint(infoItem->size());
            m_deviceList->addItem(listItem);
            m_deviceList->setItemWidget(listItem, infoItem);
            infoItem->setProperty("path", partition.path);
            if (partition.path == this->property("last_path").toString()) {
                infoItem->setCheck(true);
                m_deviceList->setCurrentItem(listItem);
                hasPartitionSelected = true;
            }
        }

        start->setDisabled(!hasPartitionSelected);

        if (!hasPartitionSelected) {
            this->setProperty("last_path", "");
        }
    });

//    connect(m_deviceList, &DeviceListWidget::itemClicked,
//    this, [ = ](QListWidgetItem * current) {
//        DeviceInfoItem *infoItem = qobject_cast<DeviceInfoItem *>(m_deviceList->itemWidget(current));
//        if (infoItem) { infoItem->setCheck(true); }
//        this->setProperty("last_path", infoItem->property("path").toString());
//    });

    connect(m_deviceList, &DeviceListWidget::currentItemChanged,
    this, [ = ](QListWidgetItem * current, QListWidgetItem * previous) {
        DeviceInfoItem *infoItem = qobject_cast<DeviceInfoItem *>(m_deviceList->itemWidget(previous));
        if (infoItem) {
            infoItem->setCheck(false);
        }

        infoItem = qobject_cast<DeviceInfoItem *>(m_deviceList->itemWidget(current));
        if (infoItem) {
            infoItem->setCheck(true);
            this->setProperty("last_path", infoItem->property("path").toString());
            start->setDisabled(false);
        }
    });

    connect(start, &SuggestButton::clicked, this, [ = ] {
        QString path = this->property("last_path").toString();
        qDebug() << "Select usb device" << path;
        emit this->deviceSelected(path, m_formatDiskCheck->isChecked());
    });
}
