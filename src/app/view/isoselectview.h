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

#include <DFrame>
#include <DLabel>
#include <DPushButton>
#include "dropframe.h"

DWIDGET_USE_NAMESPACE
//class QLabel;
class SuggestButton;

class ISOSelectView : public DFrame
{
    Q_OBJECT

    Q_PROPERTY(QString isoFilePath READ isoFilePath)
public:
    explicit ISOSelectView(DWidget *parent = 0);

    inline QString isoFilePath() const
    {
        return m_isoFilePath;
    }

signals:
    void requestVerfiyISOFile(const QString &file);
    void isoFileSelected();

public slots:
    void onFileSelected(const QString &file);
    void onFileVerfiyFinished(bool ok);
    void slot_ThemeChange();

private:
    DLabel          *m_hits         = nullptr;
    DLabel          *m_fileLabel    = nullptr;
//    QLabel          *m_stateLabel    = nullptr;
    DLabel          *m_fileSelect   = nullptr;
//    SuggestButton   *m_nextSetp     = nullptr;
    DPushButton   *m_nextSetp     = nullptr;
    DLabel          *m_title     = nullptr;
    DropFrame       *isoPanel     = nullptr;
    DLabel          *spliter     = nullptr;
    QString         m_isoFilePath;

};

