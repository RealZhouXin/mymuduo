#include <iostream>
#include "muduo/base/Thread.h"
#include "muduo/base/TimeStamp.h"
#include "muduo/net/EventLoop.h"
#include <sys/timerfd.h>

muduo::net::EventLoop* g_loop;

void timeout() {
    printf("time out!\n");
    g_loop->quit();
}

int main() {
    muduo::net::EventLoop loop;
    g_loop = &loop;
    
}