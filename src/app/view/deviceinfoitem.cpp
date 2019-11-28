/*
 * Copyright (C) 2017 ~ 2018 Wuhan Deepin Technology Co., Ltd.
 *
 * Author:     Iceyer <me@iceyer.net>
 *
 * Maintainer: Iceyer <me@iceyer.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "deviceinfoitem.h"
#include "widgetutil.h"

#include <DApplication>
#include <DProgressBar>
#include <DApplicationHelper>

#include <QDebug>
#include <QHBoxLayout>
#include <QPixmap>
#include <QPainter>

DeviceDelegate::DeviceDelegate(QObject *parent)
    : QItemDelegate(parent)
{

}

void DeviceDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
// do nothing
//    return QItemDelegate::paint (painter, option, index);
}


DeviceInfoItem::DeviceInfoItem(const QString &name, const QString &device,
                               const QString &cap, int percent, DWidget *parent)
    : DWidget(parent)
{
    s_removeDevice = WidgetUtil::getDpiPixmap(":/theme/light/image/drive.svg", this);
//    s_selectDevice = WidgetUtil::getDpiPixmap(":/theme/light/image/drive-select.svg", this);

//    this->setStyleSheet(WidgetUtil::getQss(":/theme/light/DeviceInfoItem.theme"));

    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(9, 0, 6, 0);
    mainLayout->setSpacing(0);

    m_deviceIcon = new DLabel;
    m_deviceIcon->setObjectName("DeviceInfoIcon");
    m_deviceIcon->setPixmap(s_removeDevice);

    auto m_deviceLabel = new DLabel;
    m_deviceLabel->setObjectName("DeviceInfoLabel");
    m_deviceLabel->setText(name);
    m_deviceLabel->setFixedHeight(20);
    QFont qf = m_deviceLabel->font();
    qf.setPixelSize(14);
    m_deviceLabel->setFont(qf);

    auto m_deviceDevName = new DLabel;
    m_deviceDevName->setObjectName("DeviceInfoDevName");
    m_deviceDevName->setText(QString("%1").arg(device));
    m_deviceDevName->setFixedHeight(18);
    qf = m_deviceDevName->font();
    qf.setPixelSize(12);
    m_deviceDevName->setFont(qf);
    m_deviceDevName->setAlignment(Qt::AlignCenter);
    auto m_deviceCapacity = new DLabel;
    m_deviceCapacity->setObjectName("DeviceInfoCapacity");
    m_deviceCapacity->setText(cap);
    m_deviceCapacity->setFixedHeight(18);
    qf = m_deviceCapacity->font();
    qf.setPixelSize(10);
    m_deviceCapacity->setFont(qf);
    m_deviceCapacity->setAlignment(Qt::AlignCenter);

    auto m_deviceCapacityBar = new DProgressBar;
    m_deviceCapacityBar->setObjectName("DeviceInfoCapacityBar");
    m_deviceCapacityBar->setTextVisible(false);
    m_deviceCapacityBar->setFixedSize(292, 4);
    m_deviceCapacityBar->setValue(percent);
//    m_deviceCapacityBar->setStyleSheet(".QProgressBar{background - color: rgba(0, 0, 0, 0.05);border: solid 1px rgba(0, 0, 0, 0.03);border-radius: 1.5px;}"
//                                       ".QProgressBar::chunk{background - color:#2ca7f8;border-radius: 1.5px;}");


//    m_radiobutton = new QRadioButton;
//    m_radiobutton->setFocusPolicy(Qt::NoFocus);
//    m_radiobutton->setChecked(false);
//    m_radiobutton->setFixedSize(20, 20);
//    m_radiobutton->hide();


    m_fillingposition    = new DLabel;
//    m_fillingposition->setFixedSize(30, 30);

    auto m_bodywidget = new DWidget;
    auto bodyLayout = new QVBoxLayout(m_bodywidget);
    bodyLayout->setMargin(0);
    auto m_middlewidget = new DWidget;
    m_middlewidget->setFixedWidth(292);
    m_middlewidget->setFixedHeight(18);
    auto middleLayout = new QHBoxLayout(m_middlewidget);
    middleLayout->setMargin(0);

    middleLayout->addWidget(m_deviceDevName, 0, Qt::AlignLeft);
    middleLayout->addWidget(m_deviceCapacity, 0, Qt::AlignRight);

    bodyLayout->addSpacing(6);
    bodyLayout->addWidget(m_deviceLabel, 0, Qt::AlignLeft);
    bodyLayout->addSpacing(0);
    bodyLayout->addWidget(m_middlewidget, 0, Qt::AlignLeft);
    bodyLayout->addSpacing(5);
    bodyLayout->addWidget(m_deviceCapacityBar, 0, Qt::AlignCenter);
    bodyLayout->addSpacing(9);

    mainLayout->addWidget(m_deviceIcon, 0, Qt::AlignLeft);
    mainLayout->addSpacing(7);
    mainLayout->addWidget(m_bodywidget, 0, Qt::AlignLeft);
//    mainLayout->addWidget(m_deviceLabel, 0, Qt::AlignCenter);
//    mainLayout->addSpacing(10);
//    mainLayout->addWidget(m_deviceDevName, 0, Qt::AlignCenter);
//    mainLayout->addStretch();
//    mainLayout->addWidget(m_deviceCapacity, 0, Qt::AlignCenter);
//    mainLayout->addSpacing(10);
//    mainLayout->addWidget(m_deviceCapacityBar, 0, Qt::AlignCenter);
    mainLayout->addSpacing(10);
//    mainLayout->addWidget(m_radiobutton, 0, Qt::AlignRight);
    mainLayout->addWidget(m_fillingposition, 0, Qt::AlignRight);

    setFixedSize(400, 60);

    setProperty("needformat", false);

//    this->setAutoFillBackground(true);
//    pa.setColor(QPalette::Background, QColor(0, 0, 0, 13));
//    this->setPalette(pa);

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
    this, [ = ] {
        DPalette pa;
        DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
        if (themeType == DGuiApplicationHelper::LightType)
        {
            pa = m_deviceLabel->palette();
            pa.setColor(DPalette::Text, QColor("#414D68"));
            m_deviceLabel->setPalette(pa);
            pa = m_deviceDevName->palette();
            pa.setColor(DPalette::Text, QColor("#8AA1B4"));
            m_deviceDevName->setPalette(pa);
            pa = m_deviceCapacity->palette();
            pa.setColor(DPalette::Text, QColor("#8AA1B4"));
            m_deviceCapacity->setPalette(pa);
        } else if (themeType == DGuiApplicationHelper::DarkType)
        {
            pa = m_deviceLabel->palette();
            pa.setColor(DPalette::Text, QColor("#C0C6D4"));
            m_deviceLabel->setPalette(pa);
            pa = m_deviceDevName->palette();
            pa.setColor(DPalette::Text, QColor("#6D7C88"));
            m_deviceDevName->setPalette(pa);
            pa = m_deviceCapacity->palette();
            pa.setColor(DPalette::Text, QColor("#6D7C88"));
            m_deviceCapacity->setPalette(pa);
        }
    });

    emit DGuiApplicationHelper::instance()->themeTypeChanged(DGuiApplicationHelper::instance()->themeType());
}

DeviceInfoItem::DeviceInfoItem(DWidget *parent) :
    DeviceInfoItem("Remove Device", "NULL", "0/0G", 0, parent)
{
    setProperty("needformat", false);
}

void DeviceInfoItem::paintEvent(QPaintEvent *event)
{
    QColor baccolor;
    DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
    if (themeType == DGuiApplicationHelper::LightType) {
        baccolor = QColor(0, 0, 0, 8);
    } else if (themeType == DGuiApplicationHelper::DarkType) {
        baccolor = QColor(255, 255, 255, 13);
    }
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(baccolor));
    painter.setPen(Qt::transparent);
    QRect rect = this->rect();
    rect.setWidth(rect.width() - 1);
    rect.setHeight(rect.height() - 1);
    painter.drawRoundedRect(rect, 8, 8);
    DWidget::paintEvent(event);
}

QPixmap DeviceInfoItem::renderSVG(const QString &filePath, const QSize &size)
{
    QImageReader reader;
    QPixmap pixmap;

    reader.setFileName(filePath);

    if (reader.canRead()) {
        const qreal ratio = qApp->devicePixelRatio();
        reader.setScaledSize(size * ratio);
        pixmap = QPixmap::fromImage(reader.read());
        pixmap.setDevicePixelRatio(ratio);
    } else {
        pixmap.load(filePath);
    }

    return pixmap;
}

void DeviceInfoItem::setCheck(bool flag)
{
//    m_deviceIcon->setPixmap(flag ? s_selectDevice : s_removeDevice);
//    m_radiobutton->setChecked(flag);
    if (flag) {
        QPixmap pixmap = renderSVG(":/theme/light/image/select_active.svg", QSize(28, 28));
//        pixmap.setDevicePixelRatio(m_fillingposition->devicePixelRatioF());
//        pixmap.setDevicePixelRatio(qApp->devicePixelRatio());
//        pixmap = pixmap.scaled(pixmap.width(), pixmap.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        m_fillingposition->setPixmap(pixmap);
//        m_radiobutton->show();
//        m_fillingposition->hide();
    } else {
        m_fillingposition->clear();
//        m_radiobutton->hide();
//        m_fillingposition->show();
    }
}

bool DeviceInfoItem::needFormat() const
{
    return property("needformat").toBool();
}

void DeviceInfoItem::setNeedFormat(bool format)
{
    setProperty("needformat", format);
}
