#include "thread.hpp"

Thread::~Thread()
{
    this->stop();
}
Thread::Thread(const std::string& name, std::function<void()> funcToExecute)
    : _funct(std::move(funcToExecute))
{
    _stream.setPrefix(name + ": ");
}

void Thread::start()
{
    if (_t.joinable())
    {
        _stream << "[ERROR] Thread already running [ERROR]";
        stop();
    }
    _stream << "Start" << std::endl;
    _t = std::thread(_funct);
}

void Thread::stop()
{

    if (!_t.joinable())
        return;
    _t.join();
    _stream << "Stop" << std::endl;
}