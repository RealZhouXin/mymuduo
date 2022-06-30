#pragma once

#include "base/noncopyable.h"
#include "base/TimeStamp.h"

#include <functional>
#include <memory>

namespace muduo {
namespace net {
class EventLoop;

//The file description could be a socket, an eventfd, 
//a timerfd or a signalfd
class Channel : noncopyable {
public:
    using EventCallback = std::function<void()>;


    Channel(EventLoop* loop, int fd);
    ~Channel();
    void handleEvent();
    void setReadCallback(const EventCallback& cb) {
        readCallback_ = cb;
    }
    void setWriteCallback(const EventCallback& cb) {
        writeCallback_ = cb;
    }
    void setErrorCallback(const EventCallback& cb) {
        errorCallback_ = cb;
    }

    int fd() const {return fd_;}
    int events() const {return events_;}
    void set_revents(int revt) { revents_ = revt;}
    bool isNoneEvent() const {return events_ == kNoneEvent;}

    void enableReading() {events_ |= kReadEvent; update();}

    //for Poller
    int index() { return index_;}
    void set_index(int idx) {index_ = idx;}

    EventLoop* ownerLoop() {return loop_;}


private:
    void update();
    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    EventLoop* loop_;
    const int fd_;
    int events_;
    int revents_;
    int index_;//use by Poller

    EventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback errorCallback_;
};


}
}