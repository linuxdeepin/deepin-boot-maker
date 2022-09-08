// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <DListWidget>
#include <DWidget>
#include <DLabel>

#include <QRadioButton>
#include <QItemDelegate>
#include <QImageReader>

DWIDGET_USE_NAMESPACE

class DeviceDelegate : public QItemDelegate
{
    Q_OBJECT

public:

    DeviceDelegate(QObject *parent = nullptr);
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class DeviceInfoItem : public DWidget
{
    Q_OBJECT
public:
    explicit DeviceInfoItem(const QString &name,
                            const QString &device,
                            const QString &cap,
                            int percent, DWidget *parent = Q_NULLPTR);
    explicit DeviceInfoItem(DWidget *parent = Q_NULLPTR);

    void setCheck(bool flag);
    bool isChecked();
    bool needFormat() const;
    void setNeedFormat(bool format);

protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QPixmap renderSVG(const QString &filePath, const QSize &size);
    bool checked            = false;
    DLabel *m_deviceIcon    = nullptr;
    DLabel *m_fillingposition    = nullptr;
    QString m_filepath;
    QPixmap s_removeDevice;
};

