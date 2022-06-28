#include "TimeStamp.h"
#include <cstddef>
#include <fmt/core.h>
#include <time.h>

using namespace muduo;

TimeStamp TimeStamp::now() {
    return TimeStamp(time(NULL));
}
string TimeStamp::toString() const {
    tm *tm_time = localtime(&microSecondsSinceEpoch_);
    string res = fmt::format("{:04d}/{:02d}/{:02d} {:02d}:{:02d}:{:02d}", tm_time->tm_year + 1900,
                             tm_time->tm_mon + 1, tm_time->tm_mday, tm_time->tm_hour, tm_time->tm_min, tm_time->tm_sec);
    return res;
}