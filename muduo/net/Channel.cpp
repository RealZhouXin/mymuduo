#include "net/Channel.h"

#include "net/EventLoop.h"
#include <poll.h>
#include <sstream>
#include <sys/poll.h>
#include <fmtlog/fmtlog.h>
using namespace muduo;
using namespace muduo::net;

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop *loop, int fd) :
    loop_(loop), fd_(fd),
    events_(0),
    revents_(0),
    index_(-1) {
}
Channel::~Channel() {
    
}

void Channel::update() {
    loop_->updateChannel(this);
}

void Channel::handleEvent() {
    if(revents_ & POLLNVAL) {
        logw("Channel::handle_event() POLLNVAL");
    }
    if(revents_ & (POLLERR | POLLNVAL)) {
        if(errorCallback_) errorCallback_();
    }
    if(revents_ & (POLLIN | POLLPRI | POLLRDHUP)) {
        if(readCallback_) readCallback_();
    }
    if(revents_ & (POLLOUT)) {
        if(writeCallback_) writeCallback_();
    }
}
