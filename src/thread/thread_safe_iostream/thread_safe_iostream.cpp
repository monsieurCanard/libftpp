#include "thread_safe_iostream.hpp"

thread_local ThreadSafeIOStream threadSafeCout;
std::mutex                      ThreadSafeIOStream::_mutex;

void ThreadSafeIOStream::setPrefix(const std::string& prefix)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _prefix = prefix;
}

ThreadSafeIOStream& ThreadSafeIOStream::operator<<(std::ostream& (*funct)(std::ostream&))
{
    std::lock_guard<std::mutex> lock(_mutex);
    flushBuffer();
    funct(std::cout);
    return *this;
}

void ThreadSafeIOStream::flushBuffer()
{
    std::string content = _buffer.str();
    if (!content.empty())
    {
        std::cout << _prefix << content;
        _buffer.str("");
        _buffer.clear();
    }
}