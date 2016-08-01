/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dcolorpicker.h"
#include "dthememanager.h"
#include <QDebug>
#include <QLinearGradient>
#include <QPainter>
#include <QMouseEvent>

DWIDGET_BEGIN_NAMESPACE

DColorPicker::DColorPicker(int row, int column, int cellSize,
                           int spacing, int margin,
                           QWidget *parent) :
    QFrame(parent),
    m_row(row),
    m_column(column),
    m_cellSize(cellSize),
    m_spacing(spacing),
    m_margin(margin),
    m_mousePressPos(-1, -1),
    m_selectedBorderColor(Qt::white)
{
    setObjectName("DColorPicker");
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    D_THEME_INIT_WIDGET(DColorPicker);

    resize();
}

DColorPicker::DColorPicker(int margin, QWidget *parent):
    QFrame(parent),
    m_row(12),
    m_column(19),
    m_cellSize(12),
    m_spacing(2),
    m_margin(margin),
    m_mousePressPos(-1, -1),
    m_selectedBorderColor(Qt::white)
{
    setObjectName("DColorPicker");
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    D_THEME_INIT_WIDGET(DColorPicker);

    resize();

    addColorGradient(QPoint(1, 1), QPoint(12, 1), Qt::black, Qt::white);
    addColorGradient(QPoint(1, 2), QPoint(6, 7), "#000011", "#00FFFF");
    addColorGradient(QPoint(1, 8), QPoint(6, 13), "#330000", "#33FFFF");
    addColorGradient(QPoint(1, 14), QPoint(6, 19), "#660000", "#66FFFF");
    addColorGradient(QPoint(7, 2), QPoint(12, 7), "#990000", "#99FFFF");
    addColorGradient(QPoint(7, 8), QPoint(12, 13), "#CC0000", "#CCFFFF");
    addColorGradient(QPoint(7, 14), QPoint(12, 19), "#FF0000", "#FFFFFF");
}

QColor DColorPicker::currentColor() const
{
    return m_currentColor;
}

QColor DColorPicker::at(int r, int c)
{
    return atForPos(QPoint(m_margin+(r-1)*(m_spacing+m_cellSize),
                           m_margin+(c-1)*(m_spacing+m_cellSize)));
}

QColor DColorPicker::atForPos(const QPoint &pos)
{
    if(rect().contains(pos))
        return grab(QRect(pos, QSize(1, 1))).toImage().pixel(0, 0);
    else
        return QColor::Invalid;
}

int DColorPicker::row() const
{
    return m_row;
}

int DColorPicker::column() const
{
    return m_column;
}

int DColorPicker::cellSize() const
{
    return m_cellSize;
}

int DColorPicker::spacing() const
{
    return m_spacing;
}

int DColorPicker::margin() const
{
    return m_margin;
}

const DColorPicker::Gradient &DColorPicker::colorGradient(int index) const
{
    return m_gradientList[index];
}

QColor DColorPicker::selectedBorderColor() const
{
    return m_selectedBorderColor;
}

void DColorPicker::setCurrentColor(QColor currentColor)
{
    if (m_currentColor == currentColor)
        return;

    m_currentColor = currentColor;

    update();

    emit currentColorChanged(currentColor);
}

void DColorPicker::setRow(int row)
{
    if (m_row == row)
        return;

    m_row = row;
    resize();
    emit rowChanged(row);
}

void DColorPicker::setColumn(int column)
{
    if (m_column == column)
        return;

    m_column = column;
    resize();
    emit columnChanged(column);
}

void DColorPicker::setCellSize(int cellSize)
{
    if (m_cellSize == cellSize)
        return;

    m_cellSize = cellSize;
    resize();

    emit cellSizeChanged(cellSize);
}

void DColorPicker::setSpacing(int spacing)
{
    if(spacing == m_spacing)
        return;

    m_spacing = spacing;
    resize();

    emit spacingChanged(spacing);
}

void DColorPicker::setMargin(int margin)
{
    if(margin == m_margin)
        return;

    m_margin = margin;
    resize();

    emit marginChanged(margin);
}

int DColorPicker::addColorGradient(const QPoint &begin, const QPoint &end,
                                    const QColor &startColor, const QColor &endColor)
{
    if(begin.x()<0||begin.y()<0||end.x()>m_row||end.y()>m_column
            ||!startColor.isValid()||!endColor.isValid()){
        return -1;
    }

    m_gradientList<<Gradient{begin, end, startColor, endColor};
    update();
    return m_gradientList.count()-1;
}

void DColorPicker::setColorGradient(int index, const QPoint &begin, const QPoint &end,
                                    const QColor &startColor, const QColor &endColor)
{
    if(begin.x()<0||begin.y()<0||end.x()>m_row||end.y()>m_column
            ||!startColor.isValid()||!endColor.isValid()){
        return;
    }

    m_gradientList[index] = Gradient{begin, end, startColor, endColor};
    update();
}

void DColorPicker::setSelectedBorderColor(QColor selectedColor)
{
    if (m_selectedBorderColor == selectedColor)
        return;

    m_selectedBorderColor = selectedColor;
    update();

    emit selectedBorderColorChanged(selectedColor);
}

void DColorPicker::paintEvent(QPaintEvent *e)
{
    QPainter pa(this);

    foreach (const Gradient &gradient, m_gradientList) {
        int row = gradient.end.x()-gradient.begin.x()+1;
        int column = gradient.end.y()-gradient.begin.y()+1;

        for(int i=0; i<row; ++i){
            for(int j=0; j<column; ++j){
                int top_margin = (height()-m_row*(m_cellSize+m_spacing)-m_spacing)/2;
                int left_margin = (width()-m_column*(m_cellSize+m_spacing)-m_spacing)/2;

                QRect rect(left_margin+(j+gradient.begin.y()-1)*(m_cellSize+m_spacing),
                           top_margin+(i+gradient.begin.x()-1)*(m_cellSize+m_spacing),
                           m_cellSize, m_cellSize);

                QColor c = gradient.startColor.toRgb();
                QColor c2 = gradient.endColor.toRgb();

                if(row>1&&column>1){
                    c.setRedF(c.redF()+(c2.redF()-c.redF())*i/(row-1));
                    c.setGreenF(c.greenF()+(c2.greenF()-c.greenF())*j/(column-1));
                    c.setBlueF(c.blueF()+(c2.blueF()-c.blueF())*i/(row-1));
                }else if(row>1){
                    c.setRedF(c.redF()+(c2.redF()-c.redF())*i/(row-1));
                    c.setGreenF(c.greenF()+(c2.greenF()-c.greenF())*i/(row-1));
                    c.setBlueF(c.blueF()+(c2.blueF()-c.blueF())*i/(row-1));
                }else if(column>1){
                    c.setRedF(c.redF()+(c2.redF()-c.redF())*j/(column-1));
                    c.setGreenF(c.greenF()+(c2.greenF()-c.greenF())*j/(column-1));
                    c.setBlueF(c.blueF()+(c2.blueF()-c.blueF())*j/(column-1));
                }

                pa.fillRect(rect, c);

                if(m_currentColor.isValid()&&c.name() == m_currentColor.name()){
                    pa.setPen(m_selectedBorderColor);
                    rect.setX(rect.x()-1);
                    rect.setY(rect.y()-1);
                    rect.setWidth(m_cellSize+1);
                    rect.setHeight(m_cellSize+1);
                    pa.drawRect(rect);
                }
            }
        }
    }

    QFrame::paintEvent(e);
}

void DColorPicker::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        m_mousePressPos = e->pos();
        e->accept();
    }
}

void DColorPicker::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        if(e->pos() == m_mousePressPos){
            int tmp_x = (m_mousePressPos.x()-m_margin)%(m_spacing+m_cellSize);
            int tmp_y = (m_mousePressPos.y()-m_margin)%(m_spacing+m_cellSize);

            if(QRect(0,0,m_cellSize,m_cellSize).contains(tmp_x, tmp_y))
                setCurrentColor(atForPos(e->pos()));
        }
        e->accept();
    }
}

void DColorPicker::resize()
{
    setMinimumSize((m_column-1)*(m_cellSize+m_spacing)+m_cellSize+m_margin*2,
                 (m_row-1)*(m_cellSize+m_spacing)+m_cellSize+m_margin*2);
}

DWIDGET_END_NAMESPACE
