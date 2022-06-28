#pragma once

#include "copyable.h"
#include <iostream>
#include "Types.h"

namespace muduo {
class TimeStamp : public copyable {
public:
    TimeStamp() :
        microSecondsSinceEpoch_(0) {
    }
    explicit TimeStamp(int64_t ms) : microSecondsSinceEpoch_(ms){}
    void swap(TimeStamp& that) {
        std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
    }
    //output
    string toString() const;

    //get time of now
    static TimeStamp now();

private:
    int64_t microSecondsSinceEpoch_;
};
} // namespace muduo