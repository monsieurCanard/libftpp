#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <stddef.h>
#include <string.h>

#include <stdexcept>
#include <string>
#include <vector>

#include "../ring_buffer/ring_buffer.hpp"

/*
 * @note Expected Message format [type(int)][sizeofData(size_t)][data]
 */
class Message
{
public:
    using Type = int;

private:
    RingBuffer _buffer;
    Type       _type;

public:
    Message(Type type);
    Message() {}
    void setType();

    template <typename T>
    Message& operator>>(T& value)
    {
        try
        {
            _buffer.pop(sizeof(int));
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

    std::vector<unsigned char> popData();
    std::vector<unsigned char> getData() const;

    Message::Type type() const;
    RingBuffer&   data();
    bool          isComplet();
    void          reset();
};
#endif