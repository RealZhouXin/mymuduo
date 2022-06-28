#pragma once

namespace muduo {
class noncopyable {
public:
    //所有继承了noncopyable类的对象，都会包含一个noncopyable对象，这样在拷贝构造的时候，也会对noncopyable对象进行
    //拷贝构造，就会引发错误
    noncopyable(const noncopyable &) = delete;
    void operator=(const noncopyable &) = delete;

protected:
    noncopyable() = default;
    ~noncopyable() = default;
};
} // namespace muduo