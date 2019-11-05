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

#include "suggestbutton.h"

#include "widgetutil.h"
#include <QPainter>
#include <QDebug>

SuggestButton::SuggestButton(DWidget *parent) :
    DPushButton(parent),
    israised(true)
{
//    this->setStyleSheet(WidgetUtil::getQss(":/theme/light/SuggestButton.theme"));
//    this->setFixedSize(QPixmap(":/theme/light/image/suggest_button_hover.svg").size());
    this->setFixedSize(310, 36);
    m_filletradii = 8;
}

//void SuggestButton::mSetText(QString text)
//{
//    m_text = text;
//}

void SuggestButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    if (!this->isEnabled()) {
        painter.setBrush(QBrush(QColor(0, 0, 0, 13)));
    } else {
        if (israised)
            painter.setBrush(QBrush(QColor("#E3E3E3")));
        else
            painter.setBrush(QBrush(QColor("#00BFFF")));
    }
    painter.setPen(Qt::transparent);
    QRect rect = this->rect();
    rect.setWidth(rect.width() - 1);
    rect.setHeight(rect.height() - 1);
    painter.drawRoundedRect(rect, m_filletradii, m_filletradii);
    QFont qf = font();
    qf.setPixelSize(14);
    painter.setFont(qf);
//    painter.setPen(Qt::black);
    painter.setPen(QColor("#414D68"));
    painter.drawText(0, 0, this->width(), this->height(), Qt::AlignCenter, text());
    //    DPushButton::paintEvent(event);
}

void SuggestButton::enterEvent(QEvent *e)

{
    israised = false;
    repaint();     //重新绘制按钮
}

void SuggestButton::leaveEvent(QEvent *e)
{
    israised = true;
    repaint();
}

void SuggestButton::mouseEvent(QMouseEvent *e)
{
    float  w = this->width();
    float  h = this->height();
    int  x = e->x();
    int  y = e->y();
    float k = h / w; //斜率
    if ( y > -k * x + h / 2 &&
            y >= k * x - h / 2 &&
            y <= k * x + h / 2 &&
            y <= -k * x + 3 * h / 2) {
        israised = false;
    } else {
        israised = true;
    }
    repaint();
}
