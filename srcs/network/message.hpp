#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <stddef.h>

#include <vector>
class Message
{
private:
    int                        _type;
    std::vector<unsigned char> _buffer;
    size_t                     _cursor = 0;

public:
    using Type = int;
    Message(Type type) : _type(type) {}

    template <typename T>
    Message& operator>>(T& value)
    {
        if (sizeof(T) + _cursor > _buffer.size())
            throw std::runtime_error("Read out of Buffer !");
        memcpy(&value, _buffer.data() + _cursor, sizeof(T));
        _cursor += sizeof(T);
        return *this;
    }

    template <typename T>
    Message& operator<<(const T& value)
    {

        const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&value);
        _buffer.insert(_buffer.end(), ptr, ptr + sizeof(T));
        return *this;
    }

    Type type() const
    {
        return _type;
    }

    const std::vector<unsigned char>& data() const
    {
        return _buffer;
    }

    void reset()
    {
        _cursor = 0;
    }
};
#endif