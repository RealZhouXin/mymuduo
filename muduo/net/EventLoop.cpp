#include "net/EventLoop.h"
#include "net/Poller.h"
#include "base/CurrentThread.h"
#include <cassert>
#include <cstddef>
#include <fmtlog/fmtlog.h>
#include <base/Types.h>
#include <poll.h>
#include <thread>
using namespace muduo;
using namespace muduo::net;

__thread EventLoop* t_loopInThisThread = 0;
const int kPollTimeMs = 10000;

EventLoop::EventLoop() : looping_(false), threadId_(CurrentThread::tid()) {
    //每个线程只能有一个EventLoop, 因此在构造的时候要检测当前线程是否有其他eventloop
    logi("EventLoop created");
    if(t_loopInThisThread) {
        loge("Another eventLoop exit in this thread");
    } else {
        t_loopInThisThread = this;
    }      
}
EventLoop::~EventLoop() {
    assert(!looping_);
    t_loopInThisThread = nullptr;
}

EventLoop* EventLoop::getEventLoopOfCurrentThread() {
    return t_loopInThisThread;
}
void EventLoop::abortNotInLoopThread() {
    loge("not in loop thread{}, curentThread:{}", threadId_, CurrentThread::tid());
    exit(1);
}
void EventLoop::loop() {
    //事件循环必须在 IO 线程进行
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;
    quit_ = false;

    while(!quit_) {
        activeChannels_.clear();
        poller_->poll(kPollTimeMs, &activeChannels_);
        for(auto &it : activeChannels_) {
            it->handleEvent();
        }
    }
    logi("EvenLoop stop lopping");
    looping_ = false;
}

void EventLoop::quit() {
    quit_ = false;
    //wakeup();
}
//由channel::update()调用
void EventLoop::updateChannel(Channel *channel) {
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    poller_->updateChannel(channel);
}

