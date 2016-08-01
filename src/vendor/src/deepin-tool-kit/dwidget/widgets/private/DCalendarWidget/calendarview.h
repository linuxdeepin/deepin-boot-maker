/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef MYCALENDARWIDGET_H
#define MYCALENDARWIDGET_H

#include <QWidget>
#include <QList>
#include <QDate>
#include <QStyleOption>
#include <QSignalMapper>

class DCalendarDBus;
class CaLunarDayInfo;
class CalendarView : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor backgroundCircleColor MEMBER m_backgroundCircleColor DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY(QColor defaultTextColor MEMBER m_defaultTextColor DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY(QColor defaultLunarColor MEMBER m_defaultLunarColor DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY(QColor festivalLunarColor MEMBER m_festivalLunarColor DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY(QColor weekendsTextColor MEMBER m_weekendsTextColor DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY(QColor weekendsLunarColor MEMBER m_weekendsLunarColor DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY(bool cellSelectable READ cellSelectable WRITE setCellSelectable NOTIFY cellSelectableChanged)

public:
    enum ShowState {
        ShowLunar = 0x01,
        ShowLunarFestivalHighlight = 0x02,
        Normal = ShowLunar | ShowLunarFestivalHighlight,
    };

private:
    enum CalendarDayType
    {
        SO_Festival = QStyleOption::SO_CustomBase + 0x01,
        SO_Weekends = QStyleOption::SO_CustomBase + 0x02,
        SO_WeekendsAndFestival = SO_Festival | SO_Weekends,
        SO_NotCurrentMonth = 0x04,
        SO_NotCurrentMonthFestival = SO_NotCurrentMonth | SO_Festival,
        SO_Default,
    };

public:
    explicit CalendarView(QWidget *parent = 0);

    int getDateType(const QDate &date) const;
    inline bool cellSelectable() const {return m_cellSelectable;}

signals:
    void dateSelected(const QDate &date, const CaLunarDayInfo &detail) const;
    void cellSelectableChanged(bool cellSelectable) const;

public slots:
    void setCurrentDate(const QDate &date);
    void setLunarVisible(bool visible);
    void setLunarFestivalHighlight(bool highlight);
    void setCellSelectable(bool selectable);

protected:
    void paintEvent(QPaintEvent *e);

private:
    int getDateIndex(const QDate &date) const;
    const QString getCellDayNum(int pos);
    const QString getLunar(int pos);
    const CaLunarDayInfo getCaLunarDayInfo(int pos) const;
    void paintCell(QWidget *cell);
    bool eventFilter(QObject *o, QEvent *e);

private slots:
    void cellClicked(QWidget *cell);
    void setSelectedCell(int index);
    void getDbusData() const;

private:
    QList<QWidget *> m_cellList;
    DCalendarDBus *m_DBusInter;
    QDate m_days[42];
    QDate m_currentDate;

    ShowState m_showState = Normal;
    int m_selectedCell = 0;
    bool m_cellSelectable = true;

    QFont m_dayNumFont;
    QFont m_dayLunarFont;

    QColor m_backgroundCircleColor = QColor(33, 147, 202);

    QColor m_defaultTextColor = Qt::black;
    QColor m_weekendsTextColor = Qt::gray;
    QColor m_selectedTextColor = Qt::white;
    QColor m_festivalTextColor = Qt::cyan;
    QColor m_notCurrentTextColor = QColor(0x55, 0x55, 0x55);

    QColor m_defaultLunarColor = Qt::black;
    QColor m_weekendsLunarColor = Qt::gray;
    QColor m_selectedLunarColor = Qt::white;
    QColor m_festivalLunarColor = Qt::cyan;
    QColor m_notCurrentLunarColor = QColor(0x55, 0x55, 0x55);

    static QQueue<int> *queue;
    static QMap<QDate, CaLunarDayInfo> *lunarCache;
    static CaLunarDayInfo *emptyCaLunarDayInfo;
};

#endif // MYCALENDARWIDGET_H
