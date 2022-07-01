#pragma once

#include "base/TimeStamp.h"
#include "base/noncopyable.h"
#include "net/Channel.h"
#include "net/EventLoop.h"
#include <map>
#include <vector>


struct pollfd;
namespace muduo{
namespace net{
class Channel;

class Poller : noncopyable{
public:
    using ChannelList = std::vector<Channel*>;
    Poller(EventLoop* loop);
    ~Poller();

    TimeStamp poll(int timeoutMS, ChannelList* activeChannels);

    void updateChannel(Channel* channel);
    void assertInLoopThread() {ownerLoop_->assertInLoopThread();}
private:
    void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;
    using PollFdList = std::vector<pollfd>;
    using ChannelMap = std::map<int, Channel*>;

    EventLoop* ownerLoop_;
    PollFdList pollfds_;
    ChannelMap channels_;
};

}

}