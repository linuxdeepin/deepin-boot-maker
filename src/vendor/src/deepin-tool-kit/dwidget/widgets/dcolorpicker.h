/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DCOLORPICKER_H
#define DCOLORPICKER_H

#include <QFrame>
#include <QGridLayout>
#include <QColor>
#include "dwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class DColorPicker : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(QColor currentColor READ currentColor WRITE setCurrentColor NOTIFY currentColorChanged)
    Q_PROPERTY(int row READ row WRITE setRow NOTIFY rowChanged)
    Q_PROPERTY(int column READ column WRITE setColumn NOTIFY columnChanged)
    Q_PROPERTY(int cellSize READ cellSize WRITE setCellSize NOTIFY cellSizeChanged)
    Q_PROPERTY(int spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)
    Q_PROPERTY(int margin READ margin WRITE setMargin NOTIFY marginChanged)
    Q_PROPERTY(QColor selectedBorderColor READ selectedBorderColor WRITE setSelectedBorderColor NOTIFY selectedBorderColorChanged)

public:
    struct Gradient{
        QPoint begin;
        QPoint end;
        QColor startColor;
        QColor endColor;
    };

    explicit DColorPicker(int row, int column, int cellSize,
                          int spacing = 1, int margin = 0,
                          QWidget *parent = 0);
    explicit DColorPicker(int margin = 0, QWidget *parent = 0);

    QColor currentColor() const;
    QColor at(int r, int c);
    QColor atForPos(const QPoint &pos);
    int row() const;
    int column() const;
    int cellSize() const;
    int spacing() const;
    int margin() const;
    const Gradient &colorGradient(int index) const;
    QColor selectedBorderColor() const;

public Q_SLOTS:
    void setCurrentColor(QColor currentColor);
    void setRow(int row);
    void setColumn(int column);
    void setCellSize(int cellSize);
    void setSpacing(int spacing);
    void setMargin(int margin);
    int addColorGradient(const QPoint &begin, const QPoint &end,
                          const QColor &startColor, const QColor &endColor);
    void setColorGradient(int index, const QPoint &begin, const QPoint &end,
                          const QColor &startColor, const QColor &endColor);

    void setSelectedBorderColor(QColor selectedBorderColor);

Q_SIGNALS:
    void currentColorChanged(QColor currentColor);
    void rowChanged(int row);
    void columnChanged(int column);
    void cellSizeChanged(int cellSize);
    void spacingChanged(int spacing);
    void startColorChanged(QColor startColor);
    void endColorChanged(QColor endColor);
    void marginChanged(int margin);
    void selectedBorderColorChanged(QColor selectedBorderColor);

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent*e);

private Q_SLOTS:
    void resize();

private:
    QColor m_currentColor;
    int m_row;
    int m_column;
    int m_cellSize;
    int m_spacing;
    int m_margin;
    QList<Gradient> m_gradientList;
    QPoint m_mousePressPos;
    QColor m_selectedBorderColor;
};

DWIDGET_END_NAMESPACE

#endif // DCOLORPICKER_H
