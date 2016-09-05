/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "calendarview.h"
#include "dcalendardbus.h"

#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QEvent>
#include <QDebug>
#include <QMessageBox>
#include <QTime>
#include <QQueue>

QQueue<int> *CalendarView::queue = nullptr;
QMap<QDate, CaLunarDayInfo> *CalendarView::lunarCache = nullptr;
CaLunarDayInfo *CalendarView::emptyCaLunarDayInfo = nullptr;

CalendarView::CalendarView(QWidget *parent) : QWidget(parent)
{
    m_DBusInter = new DCalendarDBus("com.deepin.api.LunarCalendar", "/com/deepin/api/LunarCalendar", QDBusConnection::sessionBus(), this);
    if (!queue)
        queue = new QQueue<int>;
    if (!lunarCache)
        lunarCache = new QMap<QDate, CaLunarDayInfo>;
    if (!emptyCaLunarDayInfo)
        emptyCaLunarDayInfo = new CaLunarDayInfo;
    m_dayNumFont.setPixelSize(12);
    m_dayLunarFont.setPixelSize(11);

    QHBoxLayout *headerLayout = new QHBoxLayout;
    QLocale locale;
    for (int i = 0; i != 7; ++i)
    {
        QLabel *label = new QLabel(locale.dayName(i ? i : 7, QLocale::ShortFormat));
        label->setObjectName("CalendarHeader");
        label->setAlignment(Qt::AlignCenter);
        headerLayout->addWidget(label);
    }
    headerLayout->setMargin(0);
    headerLayout->setSpacing(0);

    // cells grid
    QGridLayout *gridLayout = new QGridLayout;
    for (int r = 0; r != 6; ++r)
        for (int c = 0; c != 7; ++c) {
            QWidget *cell = new QWidget;
            cell->installEventFilter(this);
            cell->setFocusPolicy(Qt::ClickFocus);
            cell->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            gridLayout->addWidget(cell, r, c);
            m_cellList.append(cell);
        }
    gridLayout->setMargin(0);
    gridLayout->setSpacing(0);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(headerLayout);
    mainLayout->addLayout(gridLayout);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    setLayout(mainLayout);
}

int CalendarView::getDateType(const QDate &date) const
{
    const int currentIndex = getDateIndex(date);
    const CaLunarDayInfo info = getCaLunarDayInfo(currentIndex);
    const int dayOfWeek = date.dayOfWeek();
    bool weekends = dayOfWeek == 6 || dayOfWeek == 7;
    bool isCurrentMonth = m_currentDate.month() == date.month();
    bool isFestival = !info.mSolarFestival.isEmpty() || !info.mLunarFestival.isEmpty();

    int resultFlag = 0;
    if (!isCurrentMonth)
        resultFlag |= SO_NotCurrentMonth;
    if (isFestival)
        resultFlag |= SO_Festival;
    if (weekends)
        resultFlag |= SO_Weekends;

    return resultFlag;
}

void CalendarView::setCurrentDate(const QDate &date)
{
    m_currentDate = date;

    const QDate firstDay(date.year(), date.month(), 1);
    const int daysOfCal = (firstDay.dayOfWeek() % 7) + date.day() - 1;
    const int y = date.dayOfWeek() % 7;
    const int x = daysOfCal / 7;
    const int currentIndex = x * 7 + y;

    for (int i(0); i != 42; ++i)
        m_days[i] = date.addDays(i - currentIndex);

    setSelectedCell(currentIndex);
    update();
}

void CalendarView::setLunarVisible(bool visible)
{
    int state = int(m_showState);

    if (visible)
        state |= ShowLunar;
    else
        state &= ~ShowLunar;

    m_showState = ShowState(state);
    update();
}

void CalendarView::setLunarFestivalHighlight(bool highlight)
{
    int state = int(m_showState);

    if (highlight)
        state |= ShowLunarFestivalHighlight;
    else
        state &= ~ShowLunarFestivalHighlight;

    m_showState = ShowState(state);
    update();
}

void CalendarView::setCellSelectable(bool selectable)
{
    if (selectable == m_cellSelectable)
        return;
    m_cellSelectable = selectable;

    emit cellSelectableChanged(m_cellSelectable);
}

void CalendarView::paintEvent(QPaintEvent *e)
{
//    for (QWidget *w : m_cellList)
//        w->update();

    QWidget::paintEvent(e);
}

int CalendarView::getDateIndex(const QDate &date) const
{
    for (int i = 0; i != 42; ++i)
        if (m_days[i] == date)
            return i;

    return 0;
}

bool CalendarView::eventFilter(QObject *o, QEvent *e)
{
    QWidget *cell = qobject_cast<QWidget *>(o);

    if (cell && m_cellList.contains(cell)) {
        if (e->type() == QEvent::Paint) {
            paintCell(cell);
        } else if (e->type() == QEvent::MouseButtonRelease) {
            cellClicked(cell);
        }
    }

    return false;
}

const QString CalendarView::getCellDayNum(int pos)
{
    const QString result = QString::number(m_days[pos].day());

    if (m_showState & ShowLunar)
        return result + '\n';
    else
        return result;
}

const QString CalendarView::getLunar(int pos)
{
    CaLunarDayInfo info = getCaLunarDayInfo(pos);

    if (info.mTerm.isEmpty())
        return info.mLunarDayName;
    return info.mTerm;
}

const CaLunarDayInfo CalendarView::getCaLunarDayInfo(int pos) const
{
    const QDate date = m_days[pos];

    if (!date.isValid()) {
        return *emptyCaLunarDayInfo;
    }
    if (lunarCache->contains(date))
        return lunarCache->value(date);
    if (lunarCache->size() > 300)
        lunarCache->clear();

//    QTimer::singleShot(500, [this, pos] {getDbusData(pos);});
    queue->push_back(pos);
    QTimer::singleShot(300, this, SLOT(getDbusData()));

    return *emptyCaLunarDayInfo;
}

void CalendarView::getDbusData() const
{
    if (queue->isEmpty())
        return;

    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);

    const int pos = queue->head();
    queue->pop_front();
    const QDate date = m_days[pos];
    CaLunarDayInfo currentDayInfo;

    if (!lunarCache->contains(date)) {
        if (!date.isValid()) {
            return;
        }
        bool o1 = true;
        QDBusReply<CaLunarMonthInfo> reply = m_DBusInter->GetLunarMonthCalendar(date.year(), date.month(), false, o1);
        QDate cacheDate;
        cacheDate.setDate(date.year(), date.month(), 1);
        foreach(const CaLunarDayInfo & dayInfo, reply.value().mCaLunarDayInfo) {
            lunarCache->insert(cacheDate, dayInfo);
            if (date == m_currentDate) {
                currentDayInfo = dayInfo;
            }
            cacheDate = cacheDate.addDays(1);
        }
    } else {
        currentDayInfo = lunarCache->value(date);
    }

    m_cellList.at(pos)->update();

    // refersh lunar info
    if (date == m_currentDate) {
        emit dateSelected(date, currentDayInfo);
    }
}

void CalendarView::paintCell(QWidget *cell)
{
    const int pos = m_cellList.indexOf(cell);
    const int type = getDateType(m_days[pos]);
    const bool isSelectedCell = pos == m_selectedCell;
    QPainter painter(cell);

    // draw selected cell background circle
    if (isSelectedCell)
    {
        QRect fillRect = cell->rect();
        QRect squareRect = fillRect;
        if (fillRect.width() < fillRect.height())
        {
            squareRect.setTop(fillRect.top() + (fillRect.height() - fillRect.width()) / 2);
            squareRect.setWidth(fillRect.width());
            squareRect.setHeight(fillRect.width());
        }
        else
        {
            squareRect.setLeft(fillRect.left() + (fillRect.width() - fillRect.height()) / 2);
            squareRect.setWidth(fillRect.height());
            squareRect.setHeight(fillRect.height());
        }

        painter.setRenderHints(QPainter::HighQualityAntialiasing);

        painter.setBrush(QBrush(m_backgroundCircleColor));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(squareRect);
    }

    painter.setPen(Qt::SolidLine);

    // draw text of day
    if (isSelectedCell)
        painter.setPen(m_selectedTextColor);
    else
    {
        const int tType = type & 0xff;
        if (tType & SO_NotCurrentMonth)
            painter.setPen(m_notCurrentTextColor);
        else if (type == SO_Weekends)
            painter.setPen(m_weekendsTextColor);
        else
            painter.setPen(m_defaultTextColor);
    }

    painter.setFont(m_dayNumFont);
    painter.drawText(cell->rect(), Qt::AlignCenter, getCellDayNum(pos));

    // draw text of day type
    if (m_showState & ShowLunar)
    {
        if (isSelectedCell)
            painter.setPen(m_selectedLunarColor);
        else if (m_showState & ShowLunarFestivalHighlight)
        {
            const int tType = type & 0xff;
            if (m_showState & ShowLunarFestivalHighlight)
            {
                if (tType & SO_NotCurrentMonth)
                    painter.setPen(tType & SO_Festival ? m_festivalLunarColor : m_notCurrentLunarColor);
                else if (tType & SO_Festival)
                    painter.setPen(m_festivalLunarColor);
                else if (tType & SO_Weekends)
                    painter.setPen(m_weekendsLunarColor);
                else
                    painter.setPen(m_defaultLunarColor);
            } else
                painter.setPen(m_defaultLunarColor);
        }

        painter.setFont(m_dayLunarFont);
        painter.drawText(cell->rect(), Qt::AlignCenter, '\n' + getLunar(pos));
    }

    painter.end();
}

void CalendarView::cellClicked(QWidget *cell)
{
    if (!m_cellSelectable)
        return;

    const int pos = m_cellList.indexOf(cell);
    if (pos == -1)
        return;

    setSelectedCell(pos);

    // my gift eggs
    static int gift = 0;
    if (m_days[pos] == QDate(1993, 7, 28))
        if (++gift == 10)
            QMessageBox::about(this, "LinuxDeepin", "by shibowen <sbw@sbw.so> :P");
}

void CalendarView::setSelectedCell(int index)
{
    if (m_selectedCell == index)
        return;

    const int prevPos = m_selectedCell;
    m_selectedCell = index;

    m_cellList.at(prevPos)->update();
    m_cellList.at(index)->update();

    emit dateSelected(m_days[index], getCaLunarDayInfo(index));
}
