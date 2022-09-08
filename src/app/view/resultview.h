// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <DPushButton>
#include <DWidget>
#include <DLabel>
#include <DPalette>
#include <DTipLabel>

#include <QPointer>

DWIDGET_USE_NAMESPACE

class ResultView : public DWidget
{
    Q_OBJECT
public:
    explicit ResultView(DWidget *parent = nullptr);

public slots:
    void onLogLinkActivated(const QString &);
    void updateResult(quint32 error, const QString &, const QString &description);

private:
    QPointer<DLabel>  m_title;
    QPointer<DLabel> m_hitsTitle;
    QPointer<DLabel> m_resultIcon;
    QPointer<DLabel> m_logHits;
    QPointer<DPushButton> m_rebootLater;
    QPointer<DPushButton> m_rebootNow;

};
