#include <iostream>
#include "muduo/base/Thread.h"
#include "muduo/base/TimeStamp.h"

int main() {
    std::cout << muduo::TimeStamp::now().toString();
}