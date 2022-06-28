#pragma once

#include "CurrentThread.h"
#include "noncopyable.h"
#include <assert.h>
#include <pthread.h>

namespace muduo {
class MutexLock : noncopyable {
public:
    MutexLock() :
        holder_(0) {
        pthread_mutex_init(&mutex_, NULL);
    }
    ~MutexLock() {
        assert(holder_ == 0);
        pthread_mutex_destroy(&mutex_);
    }
    // must be called when locked
    bool isLockedByThisThread() const {
        return holder_ == CurrentThread::tid();
    }
    void assertLocked() {
        assert(isLockedByThisThread());
    }
    // internal usage
    void lock() {
        pthread_mutex_lock(&mutex_);
        holder_ = CurrentThread::tid();
    }
    void unlock() {
        holder_ = 0;
        pthread_mutex_unlock(&mutex_);
    }
    pthread_mutex_t *getPthreadMutex() {
        return &mutex_;
    }

private:
    pthread_mutex_t mutex_;
    pid_t holder_;
};

class MutexLockGuard : noncopyable {
public:
    explicit MutexLockGuard(MutexLock &mutex) :
        mutex_(mutex) {
        mutex_.lock();
    }
    ~MutexLockGuard() {
        mutex_.unlock();
    }

private:
    MutexLock &mutex_;
};

} // namespace muduo
