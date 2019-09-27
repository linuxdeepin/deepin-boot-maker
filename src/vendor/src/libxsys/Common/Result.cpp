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

#include "Result.h"

#include <QString>

namespace XSys {

Result::Result() {
    this->code_ = Success;
}

Result::Result(int code, const QString &errmsg, const QString &result, const QString &cmd) {
    this->code_ = code;
    this->errmsg_ = errmsg;
    this->result_ = result;
    this->cmd_ = cmd;
}

Result::Result(const Result &r) {
    this->code_ = r.code_;
    this->errmsg_ = r.errmsg_;
    this->result_ = r.result_;
    this->cmd_ = r.cmd_;
}

Result::~Result() {
}

bool Result::isSuccess() const {
    return (Success == this->code_);
}

int Result::code() const{
    return this->code_;
}

const QString& Result::cmd() const{
    return this->cmd_;
}

const QString& Result::errmsg() const{
    return this->errmsg_;
}

const QString& Result::result() const{
    return this->result_;
}


}
