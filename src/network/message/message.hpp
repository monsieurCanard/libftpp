#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <stddef.h>
#include <string.h>

#include <stdexcept>
#include <string>
#include <vector>

#include "../ring_buffer/ring_buffer.hpp"

class Message
{
private:
    int        _type;
    RingBuffer _buffer;

public:
    using Type = int;
    Message(Type type);

    bool isComplet();

    template <typename T>
    Message& operator>>(T& value)
    {
        try
        {
            _buffer.popInto(&value, sizeof(T));
        }
        catch (const std::runtime_error& e)
        {
            // TODO:
            throw;
        }
        return *this;
    }

    template <typename T>
    Message& operator<<(const T& value)
    {
        try
        {
            _buffer.pushInto(&value, sizeof(T));
        }
        catch (const std::runtime_error& e)
        {
            throw;
        }
        return *this;
    }

    Message& operator<<(const std::string& value);
    Message& operator>>(std::string& value);

    Type type() const
    {
        return _type;
    }

    void reset();
};
#endif