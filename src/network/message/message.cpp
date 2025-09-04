#include "message.hpp"

Message::Message(Type type) : _type(type) {}

Message& Message::operator<<(const std::string& value)
{
    size_t size = value.length();
    *this << size;
    try
    {
        _buffer.pushInto(value.data(), size);
    }
    catch (const std::runtime_error& e)
    {
        throw;
    }
    return *this;
}

Message& Message::operator>>(std::string& value)
{
    size_t size;
    *this >> size;
    if (size > _buffer.capacityAvailable())
        throw std::runtime_error("Read out of Buffer !");
    try
    {
        _buffer.popInto(&value, size);
    }
    catch (const std::runtime_error& e)
    {
        throw;
    }
    return *this;
}

bool Message::isComplet()
{
    try
    {
        auto   header = _buffer.peek(sizeof(int) + sizeof(size_t));
        size_t messageSize;
        memcpy(&messageSize, header.data() + sizeof(int), sizeof(size_t));
        _buffer.peek(sizeof(int) + sizeof(size_t) + messageSize);
    }
    catch (const std::runtime_error& e)
    {
        return false;
    }
    return true;
}

void Message::reset()
{
    _buffer.clear();
}