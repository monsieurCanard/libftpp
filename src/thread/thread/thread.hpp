#ifndef THREAD_HPP
#define THREAD_HPP

#include <functional>
#include <stdexcept>
#include <string>
#include <thread>

#include "../thread_safe_iostream/thread_safe_iostream.hpp"

class Thread
{
private:
    std::function<void()> _funct;
    std::thread           _t;
    ThreadSafeIOStream    _stream;

public:
    Thread(const std::string& name, std::function<void()> funcToExecute);
    ~Thread();
    void start();
    void stop();
};

#endif