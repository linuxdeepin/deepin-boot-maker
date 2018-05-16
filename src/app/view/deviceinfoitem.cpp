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

#include "widgetutil.h"

DeviceInfoItem::DeviceInfoItem(const QString &name, const QString &device,
                               const QString &cap, int percent, QWidget *parent)
    : QWidget(parent)
{
    s_removeDevice = WidgetUtil::getDpiPixmap(":/theme/light/image/drive.svg", this);
    s_selectDevice = WidgetUtil::getDpiPixmap(":/theme/light/image/drive-select.svg", this);

    this->setStyleSheet(WidgetUtil::getQss(":/theme/light/DeviceInfoItem.theme"));

    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(12, 0, 12, 0);
    mainLayout->setSpacing(0);

    m_deviceIcon = new QLabel;
    m_deviceIcon->setObjectName("DeviceInfoIcon");
    m_deviceIcon->setPixmap(s_removeDevice);

    auto m_deviceLabel = new QLabel;
    m_deviceLabel->setObjectName("DeviceInfoLabel");
    m_deviceLabel->setText(name);

    auto m_deviceDevName = new QLabel;
    m_deviceDevName->setObjectName("DeviceInfoDevName");
    m_deviceDevName->setText(QString("(%1)").arg(device));

    auto m_deviceCapacity = new QLabel;
    m_deviceCapacity->setObjectName("DeviceInfoCapacity");
    m_deviceCapacity->setText(cap);

    auto m_deviceCapacityBar = new QProgressBar;
    m_deviceCapacityBar->setObjectName("DeviceInfoCapacityBar");
    m_deviceCapacityBar->setTextVisible(false);
    m_deviceCapacityBar->setFixedSize(60, 6);
    m_deviceCapacityBar->setValue(percent);

    mainLayout->addWidget(m_deviceIcon, 0, Qt::AlignCenter);
    mainLayout->addSpacing(14);
    mainLayout->addWidget(m_deviceLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(m_deviceDevName, 0, Qt::AlignCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(m_deviceCapacity, 0, Qt::AlignCenter);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(m_deviceCapacityBar, 0, Qt::AlignCenter);

    setFixedSize(390, 60);

    setProperty("needformat", false);
}

DeviceInfoItem::DeviceInfoItem(QWidget *parent) :
    DeviceInfoItem("Remove Device", "NULL", "0/0G", 0, parent)
{
    setProperty("needformat", false);
}

void DeviceInfoItem::setCheck(bool flag)
{
    m_deviceIcon->setPixmap(flag ? s_selectDevice : s_removeDevice);
}

bool DeviceInfoItem::needFormat() const
{
    return property("needformat").toBool();
}

void DeviceInfoItem::setNeedFormat(bool format)
{
    setProperty("needformat", format);
}
