// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

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
    DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
    QColor disablecolor, raisedcolor, disraisedcolor, textcolor;
    if (themeType == DGuiApplicationHelper::LightType) {
        disablecolor = QColor(0, 0, 0, 13);
        raisedcolor = QColor("#E3E3E3");
        disraisedcolor = QColor("#00BFFF");
        textcolor = QColor("#414D68");
    } else if (themeType == DGuiApplicationHelper::DarkType) {
        disablecolor = QColor(0, 0, 0, 13);
        raisedcolor = QColor("#E3E3E3");
        disraisedcolor = QColor("#00BFFF");
        textcolor = QColor("#414D68");
    }
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    if (!this->isEnabled()) {
        painter.setBrush(QBrush(disablecolor));
    } else {
        if (israised)
            painter.setBrush(QBrush(raisedcolor));
        else
            painter.setBrush(QBrush(disraisedcolor));
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
    painter.setPen(textcolor);
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
