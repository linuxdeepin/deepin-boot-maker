// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <DWidget>
#include <DWaterProgress>

DWIDGET_USE_NAMESPACE

class ProgressView : public DWidget
{
    Q_OBJECT
public:
    explicit ProgressView(DWidget *parent = nullptr);

protected:
    virtual void timerEvent(QTimerEvent *event);

signals:
    void finish(quint32 current,quint32 error, const QString &title, const QString &description);
    void testCancel();

private:
    int m_iInterval;
    DWaterProgress* m_waterProgress;
};

