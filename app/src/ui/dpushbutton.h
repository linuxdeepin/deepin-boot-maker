/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DPUSHBUTTON_H
#define DPUSHBUTTON_H

#include <QPushButton>
class DTips;

class DPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit DPushButton(const QString& text, QWidget *parent = 0);

    void setTextColor(const QString& colorStr);
    void setImages(const QString& normal, const QString& hover, const QString& pressed);
    void setDTips(const QString& tips) {m_tipsString = tips;}
    void showDTips(bool show) {m_showDtips = show;}
    void ingonreLeaveEvent() {m_ingonreLeaveEvent = true;}
signals:
    void entered();
    void leaved();


protected:
    virtual void enterEvent( QEvent* e );
    virtual void leaveEvent(QEvent * e);

protected slots:
    void dtipPoped();
    void leaveDone();

protected:
    QString m_tipsString;
    DTips   *m_dtips;
    bool    m_showDtips;
    bool    m_dtipsPoping;
    bool    m_mouseInRect;
    bool    m_ingonreLeaveEvent;
};

#endif // DPUSHBUTTON_H
