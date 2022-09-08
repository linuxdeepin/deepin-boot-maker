// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <DFrame>
#include <DWidget>

DWIDGET_USE_NAMESPACE
class DropFrame : public DFrame
{
    Q_OBJECT
public:
    explicit DropFrame(DWidget *parent = nullptr);

    virtual void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    virtual void dragLeaveEvent(QDragLeaveEvent *event) Q_DECL_OVERRIDE;
    virtual void dropEvent(QDropEvent *) Q_DECL_OVERRIDE;
signals:
    void fileAboutAccept();
    void fileCancel();
    void fileDrop(const QString file);
private:
    void paintEvent(QPaintEvent *) override;
};
