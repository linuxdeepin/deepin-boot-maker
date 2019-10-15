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

#pragma once

#include <QWidget>
#include <QRadioButton>
#include <QListWidget>
#include <QItemDelegate>

class QLabel;
class DeviceDelegate : public QItemDelegate
{
    Q_OBJECT

public:

    DeviceDelegate(QObject *parent = 0);
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};
class DeviceInfoItem : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceInfoItem(const QString &name,
                            const QString &device,
                            const QString &cap,
                            int percent, QWidget *parent = Q_NULLPTR);
    explicit DeviceInfoItem(QWidget *parent = Q_NULLPTR);

    void setCheck(bool flag);
    bool needFormat() const;
    void setNeedFormat(bool format);

protected:
    void paintEvent(QPaintEvent *event) override;
private:
    bool checked            = false;
    QLabel *m_deviceIcon    = nullptr;
    QRadioButton *m_radiobutton = nullptr;
    QLabel *m_fillingposition    = nullptr;

    QPixmap s_removeDevice;
//    QPixmap s_selectDevice;
};

