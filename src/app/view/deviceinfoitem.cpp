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
#include <DFontSizeManager>

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

    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(9, 0, 6, 0);
    mainLayout->setSpacing(0);

    m_deviceIcon = new DLabel;
    m_deviceIcon->setObjectName("DeviceInfoIcon");
    m_deviceIcon->setPixmap(s_removeDevice);

    auto m_deviceLabel = new DLabel;
    m_deviceLabel->setObjectName("DeviceInfoLabel");
    DPalette pa = DApplicationHelper::instance()->palette(m_deviceLabel);
    QBrush brush = DApplicationHelper::instance()->palette(m_deviceLabel).text();
    pa.setBrush(DPalette::Text, brush);
    m_deviceLabel->setText(name);
    m_deviceLabel->setFixedHeight(25);
    DFontSizeManager::instance()->bind(m_deviceLabel, DFontSizeManager::T6);

    auto m_deviceDevName = new DLabel;
    m_deviceDevName->setObjectName("DeviceInfoDevName");
    m_deviceDevName->setText(QString("%1").arg(device));
    m_deviceDevName->setFixedHeight(20);
    DFontSizeManager::instance()->bind(m_deviceDevName, DFontSizeManager::T8);

    m_deviceDevName->setAlignment(Qt::AlignCenter);
    auto m_deviceCapacity = new DLabel;
    m_deviceCapacity->setObjectName("DeviceInfoCapacity");
    m_deviceCapacity->setText(cap);
    m_deviceCapacity->setFixedHeight(18);
    DFontSizeManager::instance()->bind(m_deviceCapacity, DFontSizeManager::T10);

    m_deviceCapacity->setAlignment(Qt::AlignCenter);
#if  defined(Q_OS_WIN32)||defined(Q_OS_MAC)
    m_deviceCapacity->hide();
#endif

    auto m_deviceCapacityBar = new DProgressBar;
    m_deviceCapacityBar->setObjectName("DeviceInfoCapacityBar");
    m_deviceCapacityBar->setTextVisible(false);
    m_deviceCapacityBar->setFixedSize(292, 4);
    m_deviceCapacityBar->setValue(percent);

    m_fillingposition    = new DLabel;

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
    mainLayout->addSpacing(10);
    mainLayout->addWidget(m_fillingposition, 0, Qt::AlignRight);
    setFixedSize(400, 70);
    setProperty("needformat", false);

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
    this, [ = ] {
        DPalette pa;
        DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
        if (themeType == DGuiApplicationHelper::LightType)
        {
            pa = m_deviceDevName->palette();
            pa.setColor(DPalette::Text, QColor("#8AA1B4"));
            m_deviceDevName->setPalette(pa);
            pa = m_deviceCapacity->palette();
            pa.setColor(DPalette::Text, QColor("#8AA1B4"));
            m_deviceCapacity->setPalette(pa);
        } else if (themeType == DGuiApplicationHelper::DarkType)
        {
            pa = m_deviceDevName->palette();
            pa.setColor(DPalette::Text, QColor("#6D7C88"));
            m_deviceDevName->setPalette(pa);
            pa = m_deviceCapacity->palette();
            pa.setColor(DPalette::Text, QColor("#6D7C88"));
            m_deviceCapacity->setPalette(pa);
        }

        setCheck(checked);
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
    if (flag) {
        auto icon = qobject_cast<DStyle *>(style())->standardIcon(DStyle::SP_IndicatorChecked);
        QPixmap pixmap = icon.pixmap(QSize(16, 16), QIcon::Active);
        m_fillingposition->setPixmap(pixmap);
    } else {
        m_fillingposition->clear();
    }

    checked = flag;
}

bool DeviceInfoItem::isChecked()
{
    return checked;
}

bool DeviceInfoItem::needFormat() const
{
    return property("needformat").toBool();
}

void DeviceInfoItem::setNeedFormat(bool format)
{
    setProperty("needformat", format);
}
