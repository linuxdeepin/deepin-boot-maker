/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

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

