/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DCHECKBOX_H
#define DCHECKBOX_H

#include <QWidget>
#include <QCheckBox>

class QCheckBox;
class QLabel;
class QString;
class QPushButton;

class DCheckBox : public QWidget
{
    Q_OBJECT
public:
    explicit DCheckBox(const QString& text, QWidget *parent = 0);

    void setFixedWidth(int w);
    void setDisabled(bool);
    void setChecked(bool checked);

    void setCbk(bool checked);

    bool checked() { return m_checkState == Qt::Checked; }

    Qt::CheckState checkState() const { return m_checkState; }

    void setCheckState(Qt::CheckState state) {
        m_checkState = state;
        emit stateChanged(state);
    }

Q_SIGNALS:
    void stateChanged(int);
    void clicked();

public slots:
    void click();

private:
    QPushButton     *m_checkBox;
    QLabel          *m_label;
    int             m_indicatorSize;
    Qt::CheckState  m_checkState;

    QString         m_styleChecked;
    QString         m_styleUnchecked;
};

#endif // DCHECKBOX_H
