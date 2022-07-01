#include "net/Poller.h"
#include "base/TimeStamp.h"
#include "fmtlog/fmtlog.h"
#include "net/EventLoop.h"
#include <cassert>
#include <poll.h>
#include <utility>
using namespace muduo;
using namespace muduo::net;

Poller::Poller(EventLoop *loop) : ownerLoop_(loop) {}

Poller::~Poller(){}

TimeStamp Poller::poll(int timeoutMs, ChannelList* activeChannel) {
    int numsEvents = ::poll(pollfds_.data(), pollfds_.size(), timeoutMs);
    TimeStamp now(TimeStamp::now());
    if(numsEvents > 0) {
        logi("{} events happened", numsEvents);
        fillActiveChannels(numsEvents, activeChannel);
    } else {
        loge("Poller::poll");
    }
    return now;

}

void Poller::fillActiveChannels(int numsEvents, ChannelList* activeChannels) const {
    for(auto pfd = pollfds_.begin(); pfd != pollfds_.end(); ++pfd) {
        if(pfd->revents <= 0) {
            continue;
        }
        --numsEvents;
        //根据fd找到对应的channel
        auto ch = channels_.find(pfd->fd);
        assert(ch != channels_.end());
        auto channel = ch->second;
        assert(channel->fd() == pfd->fd);
        //将fd的返回事件写到channel上
        channel->set_revents(pfd->revents);
        //pfd->revents = 0;
        activeChannels->push_back(channel);
    }
}

void Poller::updateChannel(Channel* channel) {
    assertInLoopThread();
    logi("fd = {}, events = {}", channel->fd(), channel->events());
    if(channel->index() < 0) {
        //a new one, add to pollfds_
        assert(channels_->find(channel->fd()) == channels.end());
        pollfd pfd;
        pfd.fd = channel->fd();
        pfd.events = static_cast<short>(channel->events());
        pfd.revents = 0;
        
        pollfds_.push_back(pfd);

        int idx = static_cast<int>(pollfds_.size()) - 1;
        channel->set_index(idx);
        channels_.insert(std::make_pair(pfd.fd, channel));

    } else {
        //updating existing one
        //确认channel map 上存在channel
        auto ch = channels_.find(channel->fd());
        assert(ch != channels.end());
        assert(ch->second == channel);
        //确认pollfds_数组上存在channel->fd()
        int idx = channel->index();
        assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));
        pollfd &pfd = pollfds_[idx];
        assert(pfd.fd == channel->fd() || pfd.fd = -1);
        //更新pollfd
        pfd.events = static_cast<short>(channel->events());
        pfd.revents = 0;
        if(channel->isNoneEvent()) {
            //ignore this pollfd
            pfd.fd = -1;
        }

    }
}