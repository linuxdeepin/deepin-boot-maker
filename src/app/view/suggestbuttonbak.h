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

//#include <dbasebutton.h>
#include <DWidget>
#include <DPushButton>
#include <DGuiApplicationHelper>
#include <QMouseEvent>

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

class SuggestButton : public DPushButton
{
    Q_OBJECT
public:
    explicit SuggestButton(DWidget *parent = 0);
    void mSetText(QString text);
protected:
    void paintEvent(QPaintEvent *event) override;
    void  enterEvent(QEvent *e) override;
    void  leaveEvent(QEvent *e) override;
    void  mouseEvent(QMouseEvent *e);

private:
    qreal m_filletradii;
//    QString m_text;
    bool  israised;
};

