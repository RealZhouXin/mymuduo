#include "muduo/base/Thread.h"
#include "muduo/base/TimeStamp.h"
#include "muduo/net/EventLoop.h"
#include "net/Channel.h"
#include <bits/types/struct_itimerspec.h>
#include <iostream>
#include <sys/timerfd.h>
#include <fmt/core.h>
#include <fmtlog/fmtlog.h>

muduo::net::EventLoop *g_loop;

void timeout() {
    printf("time out!\n");
    g_loop->quit();
}

int main() {
    muduo::net::EventLoop loop;
    g_loop = &loop;


    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    muduo::net::Channel channel(&loop, timerfd);
    channel.setReadCallback(timeout);
    channel.enableReading();

    struct itimerspec howlong;
    bzero(&howlong, sizeof(howlong));
    howlong.it_value.tv_sec = 5;
    ::timerfd_settime(timerfd, 0, &howlong, NULL);
    loop.loop();
    fmtlog::poll();
    ::close(timerfd);
}