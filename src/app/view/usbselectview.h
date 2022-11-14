// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once
#include "formatdialog.h"

#include <DWidget>
#include <DFrame>
#include <DLabel>
#include <bminterface.h>
#include <QListWidgetItem>

DWIDGET_USE_NAMESPACE

class DeviceInfo;
class UsbSelectView : public DWidget
{
    Q_OBJECT
public:
    explicit UsbSelectView(DWidget *parent = nullptr);

signals:
    void deviceSelected(const QString &partition, bool format);
    void finish(quint32 error, const QString &title, const QString &description);
    void backToPrevUI();
public slots:
    void getIsoFileSelectedPath(QString isoPath);

private:
    QList<DeviceInfo> m_mountDevs;
    QListWidgetItem *m_previous = nullptr;
};

