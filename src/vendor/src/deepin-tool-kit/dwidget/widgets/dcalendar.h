/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QDate>
#include <QSize>
#include <QPropertyAnimation>

#include "dwidget_global.h"
#include "dlinkbutton.h"
#include "dimagebutton.h"

class CaLunarDayInfo;
class CalendarView;

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DCalendar : public QWidget
{
    Q_OBJECT

public:
    enum SelectionMode {
        NoSelection = 0,
        SingleSelection = 1,
    };

public:
    explicit DCalendar(QWidget *parent = 0);
    ~DCalendar();

    inline QDate getCurrentDate() const {return m_currentDate;}
    inline QDate getSelectDate() const {return m_showDate;}
    inline void setAnimationDuration(const int duration) {m_animation->setDuration(duration);}
    inline void setAnimationEasingCurve(const QEasingCurve curve) {m_animation->setEasingCurve(curve);}

signals:
    void selectedDateChanged(const QDate & date);

public slots:
    void setResetButtonText(const QString & resetText);
    void setCurrentDate(const QDate &date);
    void selectDate(const QDate &date);
    void setControlPanelVisible(bool visible);
    void setDateInfoVisible(bool visible);
    void setLunarVisible(bool visible);
    void setLunarFestivalHighlight(bool highlight);
    void setSelectionMode(SelectionMode mode);
    void updateCurrentDate();
    inline void setSolarDisplayFormat(const QString &format) {m_solarDisplayFormat = format;}

private:
    bool eventFilter(QObject *o, QEvent *e) Q_DECL_OVERRIDE;

private slots:
    void showEvent(QShowEvent *) Q_DECL_OVERRIDE;
    void viewDateChanged(const QDate &date, const CaLunarDayInfo &detail);
    void aniToPrev();
    void aniToNext();

private:
    QLabel *m_detailLabel;
    QLabel *m_solarLabel;
    QLabel *m_lunarLabel;
    QLineEdit *m_yearEdt;
    QLineEdit *m_monEdt;
    DLinkButton *m_resetBtn;

    CalendarView *m_viewLeft;
    CalendarView *m_viewRight;
    CalendarView *m_viewCurrent;

    QWidget *m_topControlPanel;
    QWidget *m_solarLunarWidget;
    QWidget *m_viewOuterWidget;
    QWidget *m_viewInnerWidget;

    QPropertyAnimation *m_animation;

    QString m_solarDisplayFormat = "dddd, MMMM dd, yyyy";

    QSize m_viewSize;
    QDate m_currentDate;
    QDate m_showDate;
};

DWIDGET_END_NAMESPACE

#endif // CALENDARWIDGET_H
