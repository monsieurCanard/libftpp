#include "thread_safe_iostream.hpp"

thread_local ThreadSafeIOStream threadSafeCout;

void ThreadSafeIOStream::setPrefix(const std::string& prefix)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _prefix = prefix;
}

ThreadSafeIOStream& ThreadSafeIOStream::operator<<(std::ostream& (*funct)(std::ostream&))
{

    std::lock_guard<std::mutex> lock(_mutex);
    if (_needPrefix)
    {
        std::cout << _prefix;
        _needPrefix = false;
    }
    funct(std::cout);
    if (funct == static_cast<std::ostream& (*)(std::ostream&)>(std::endl))
    {
        _needPrefix = true;
    }
    return *this;
}