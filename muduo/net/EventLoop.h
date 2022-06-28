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
class EventLoop : noncopyable {
public:
    EventLoop();
    ~EventLoop();

    void loop();

private:
    bool looping_; /* atomic */
    const pid_t threadId_;
};
}
} // namespace muduo::net