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
