#pragma once

#include <atomic>
#include <functional>
#include <vector>

#include "base/CurrentThread.h"
#include "base/Mutex.h"
#include "base/TimeStamp.h"
#include "base/noncopyable.h"

namespace muduo {
namespace net {

class Channel;

class EventLoop : noncopyable {
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void assertInLoopThread() {
        if(!isInLoopThread()) {
            abortNotInLoopThread();
        }
    }
    bool isInLoopThread() const {
        return threadId_ == CurrentThread::tid();
    }

    EventLoop* getEventLoopOfCurrentThread();

    void updateChannel(Channel* channel);

private:
    void abortNotInLoopThread();
    bool looping_; /* atomic */
    const pid_t threadId_;
};
}
} // namespace muduo::net