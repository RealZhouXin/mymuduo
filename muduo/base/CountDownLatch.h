#pragma once

#include "Condition.h"
#include "Mutex.h"
#include "noncopyable.h"

namespace muduo {
class CountDownLatch : noncopyable {
public:
    explicit CountDownLatch(int cnt);
    void wait();
    void countDown();
    int getCount() const;

private:
    mutable MutexLock mutex_;
    Condition cond_;
    int cnt_;
};
} // namespace muduo