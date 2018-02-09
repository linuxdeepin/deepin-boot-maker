/*
 * Copyright (C) 2015 ~ 2018 Wuhan Deepin Technology Co., Ltd.
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

#include <QString>

namespace XSys {

class Result {
public:
    enum Status{
        Success = 0,
        Faiiled = 1,
    };

    Result();
    Result(int code, const QString &msg, const QString &result="", const QString &cmd="");
    Result(const Result &r);
    ~Result();

    bool isSuccess() const;
    int code() const;
    const QString& cmd() const;
    const QString& errmsg() const;
    const QString& result() const;

private:
    int code_;
    QString errmsg_;
    QString result_;
    QString cmd_;
};

}

