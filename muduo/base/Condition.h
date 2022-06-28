#pragma once
#include "Mutex.h"
#include "noncopyable.h"
#include <pthread.h>

namespace muduo {
//如果一个class要包含一个Mutex和一个Condition，Mutex应该先于Condition构造
class Condition : noncopyable {
public:
    explicit Condition(MutexLock& mutex) : mutex_(mutex) {
        pthread_cond_init(&pcond_, NULL);
    }
    ~Condition() {
        pthread_cond_destroy(&pcond_);
    }
    void wait() {
        pthread_cond_wait(&pcond_, mutex_.getPthreadMutex());
    }
    void notify() {
        pthread_cond_signal(&pcond_);
    }
    void notifyAll() {
        pthread_cond_broadcast(&pcond_);
    }

private:
    MutexLock& mutex_;
    pthread_cond_t pcond_;
};
}