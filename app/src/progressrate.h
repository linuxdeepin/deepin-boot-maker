#pragma once

#include <QObject>

class ProcessRate {
public:
    ProcessRate() {
        max = 100;
        min = 0;
        value_ = 0;
    }
    void setMaximum(qint64 m) {
        max = m;
    }
    void setMinimum(qint64 m) {
        min = m;
    }
    void setValue(qint64 v) {
        value_ = v;
    }
    qint64 maximum() {
        return max;
    }
    qint64 value() {
        return value_;
    }
    qint64 rate() {
        if(0 != max) {
            if(value_ > max)
                return 100;

            return value_ * 100.0 / max;
        }

        return 100;
    }

protected:
    qint64 max;
    qint64 min;
    qint64 value_;
};

