#include "CountDownLatch.h"

using namespace muduo;

CountDownLatch::CountDownLatch(int cnt) :
    mutex_(), cond_(mutex_), cnt_(cnt) {
}

void CountDownLatch::wait() {
    MutexLockGuard lock(mutex_);
    while (cnt_ > 0) {
        cond_.wait();
    }
}
void CountDownLatch::countDown() {
    MutexLockGuard lock(mutex_);
    if (--cnt_ == 0) {
        cond_.notify();
    }
}
int CountDownLatch::getCount() const {
    MutexLockGuard lock(mutex_);
    return cnt_;
}
