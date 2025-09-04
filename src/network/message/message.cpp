#include "message.hpp"

Message::Message(Type type) : _type(type) {}

Message::Message(Type type, std::vector<unsigned char> buff) : _type(type), _buffer(buff) {}

const std::vector<unsigned char>& Message::data() const
{
    return _buffer;
}

Message& Message::operator<<(const std::string& value)
{
    size_t size = value.length();
    *this << size;
    _buffer.insert(_buffer.end(), value.begin(), value.end());
    return *this;
}

Message& Message::operator>>(std::string& value)
{
    size_t size;
    *this >> size;
    if (size + _cursor > _buffer.size())
        throw std::runtime_error("Read out of Buffer !");
    value.assign(_buffer.begin() + _cursor, _buffer.begin() + _cursor + size);
    _cursor += size;
    return *this;
}

void Message::reset()
{
    _cursor = 0;
}