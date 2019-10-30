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

#include <QDebug>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QApplication>


#include "widgetutil.h"


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
                               const QString &cap, int percent, QWidget *parent)
    : QWidget(parent)
{
    s_removeDevice = WidgetUtil::getDpiPixmap(":/theme/light/image/drive.svg", this);
//    s_selectDevice = WidgetUtil::getDpiPixmap(":/theme/light/image/drive-select.svg", this);

//    this->setStyleSheet(WidgetUtil::getQss(":/theme/light/DeviceInfoItem.theme"));

    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(12, 0, 12, 0);
    mainLayout->setSpacing(0);

    m_deviceIcon = new QLabel;
    m_deviceIcon->setObjectName("DeviceInfoIcon");
    m_deviceIcon->setPixmap(s_removeDevice);

    auto m_deviceLabel = new QLabel;
    m_deviceLabel->setObjectName("DeviceInfoLabel");
    m_deviceLabel->setText(name);
    QFont qf = m_deviceLabel->font();
    qf.setPointSize(8);
    m_deviceLabel->setFont(qf);
    QPalette pa;
    pa.setColor(QPalette::Text, QColor("#505050"));
    m_deviceLabel->setPalette(pa);

    auto m_deviceDevName = new QLabel;
    m_deviceDevName->setObjectName("DeviceInfoDevName");
    m_deviceDevName->setText(QString("%1").arg(device));
    qf = m_deviceDevName->font();
    qf.setPointSize(8);
    m_deviceDevName->setFont(qf);
    m_deviceDevName->setAlignment(Qt::AlignCenter);
    pa.setColor(QPalette::Text, QColor("#505050"));
    m_deviceDevName->setPalette(pa);

    auto m_deviceCapacity = new QLabel;
    m_deviceCapacity->setObjectName("DeviceInfoCapacity");
    m_deviceCapacity->setText(cap);
    qf = m_deviceCapacity->font();
    qf.setPointSize(8);
    m_deviceCapacity->setFont(qf);
    m_deviceCapacity->setAlignment(Qt::AlignCenter);
    pa.setColor(QPalette::Text, QColor("#505050"));
    m_deviceCapacity->setPalette(pa);

    auto m_deviceCapacityBar = new QProgressBar;
    m_deviceCapacityBar->setObjectName("DeviceInfoCapacityBar");
    m_deviceCapacityBar->setTextVisible(false);
    m_deviceCapacityBar->setFixedSize(270, 6);
    m_deviceCapacityBar->setValue(percent);
//    m_deviceCapacityBar->setStyleSheet(".QProgressBar{background - color: rgba(0, 0, 0, 0.05);border: solid 1px rgba(0, 0, 0, 0.03);border-radius: 1.5px;}"
//                                       ".QProgressBar::chunk{background - color:#2ca7f8;border-radius: 1.5px;}");


//    m_radiobutton = new QRadioButton;
//    m_radiobutton->setFocusPolicy(Qt::NoFocus);
//    m_radiobutton->setChecked(false);
//    m_radiobutton->setFixedSize(20, 20);
//    m_radiobutton->hide();


    m_fillingposition    = new QLabel;
    m_fillingposition->setFixedSize(20, 20);

    auto m_bodywidget = new QWidget;
    auto bodyLayout = new QVBoxLayout(m_bodywidget);
    bodyLayout->setMargin(0);
    auto m_middlewidget = new QWidget;
    m_middlewidget->setFixedWidth(270);
    auto middleLayout = new QHBoxLayout(m_middlewidget);
    middleLayout->setMargin(0);

    middleLayout->addWidget(m_deviceDevName, 0, Qt::AlignLeft);
    middleLayout->addWidget(m_deviceCapacity, 0, Qt::AlignRight);

    bodyLayout->addWidget(m_deviceLabel, 0, Qt::AlignLeft);
    bodyLayout->addWidget(m_middlewidget, 0, Qt::AlignLeft);
    bodyLayout->addWidget(m_deviceCapacityBar, 0, Qt::AlignCenter);

    mainLayout->addWidget(m_deviceIcon, 0, Qt::AlignLeft);
    mainLayout->addSpacing(10);
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

    setFixedSize(390, 60);

    setProperty("needformat", false);

//    this->setAutoFillBackground(true);
//    pa.setColor(QPalette::Background, QColor(0, 0, 0, 13));
//    this->setPalette(pa);
}

DeviceInfoItem::DeviceInfoItem(QWidget *parent) :
    DeviceInfoItem("Remove Device", "NULL", "0/0G", 0, parent)
{
    setProperty("needformat", false);
}

void DeviceInfoItem::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(QColor(0, 0, 0, 13)));
    painter.setPen(Qt::transparent);
    QRect rect = this->rect();
    rect.setWidth(rect.width() - 1);
    rect.setHeight(rect.height() - 1);
    painter.drawRoundedRect(rect, 15, 15);
    QWidget::paintEvent(event);
}

void DeviceInfoItem::setCheck(bool flag)
{
//    m_deviceIcon->setPixmap(flag ? s_selectDevice : s_removeDevice);
//    m_radiobutton->setChecked(flag);
    if (flag) {
        QPixmap pixmap(":/theme/light/image/select_active.svg");
        pixmap.setDevicePixelRatio(m_fillingposition->devicePixelRatioF());
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
