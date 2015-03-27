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

