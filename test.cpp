#include <iostream>
#include "muduo/base/Thread.h"
#include "muduo/base/TimeStamp.h"
#include "muduo/net/EventLoop.h"

int main() {
    muduo::net::EventLoop loop;
    loop.loop();
}