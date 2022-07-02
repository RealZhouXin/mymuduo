#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <vector>

#include "base/CurrentThread.h"
#include "base/Mutex.h"
#include "base/TimeStamp.h"
#include "base/noncopyable.h"

namespace muduo {
namespace net {

class Channel;
class Poller;
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
    void quit();

private:
    void abortNotInLoopThread();

    using ChannelList = std::vector<Channel*>;

    bool looping_; /* atomic */
    bool quit_; /* atomic */
    const pid_t threadId_;
    std::unique_ptr<Poller> poller_;
    ChannelList activeChannels_;
};
}
} // namespace muduo::net