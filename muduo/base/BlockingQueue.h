#pragma once

#include "Condition.h"
#include "Mutex.h"
#include "noncopyable.h"
#include <assert.h>
#include <deque>

namespace muduo {
template <typename T>
class BlockingQueue : noncopyable {
public:
    using que_t = std::deque<T>;
    BlockingQueue() :
        mutex_(), notEmpty_(mutex_), queue_() {
    }
    void put(const T &x) {
        MutexLockGuard lock(mutex_);
        queue_.push_back(x);
        notEmpty_.notify();
    }
    void put(T &&x) {
        MutexLockGuard lock(mutex_);
        queue_.push_back(std::move(x));
        notEmpty_.notify();
    }
    T take() {
        MutexLockGuard lock(mutex_);
        while (queue_.empty()) {
            notEmpty_.wait();
        }
        assert(!queue_.empty());
        T front(std::move(queue_.front()));
        queue_.pop_front();
        return front;
    }
    que_t drain() {
        que_t queue;
        {
            MutexLockGuard lock(mutex_);
            queue = std::move(queue_);
            assert(queue_.empty());
        }
        return queue;
    }
    size_t size() const {
        MutexLockGuard lock(mutex_);
        return queue_.size();
    }

private:
    mutable MutexLock mutex_;
    Condition notEmpty_;
    que_t queue_;
};
} // namespace muduo