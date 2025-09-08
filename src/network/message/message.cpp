#include "message.hpp"

Message::Message(Type type) : _type(type)
{
    *this << type;
}

Message& Message::operator<<(const std::string& value)
{
    size_t size = value.length();
    *this << size;
    try
    {
        _buffer.pushInto(value.c_str(), size);
    }
    catch (const std::runtime_error& e)
    {
        throw;
    }
    return *this;
}

Message& Message::operator>>(std::string& value)
{
    if (_buffer.size() < sizeof(int) + sizeof(size_t))
        throw std::runtime_error("No data in buffer");

    try
    {
        auto   header = _buffer.peek(sizeof(int) + sizeof(size_t));
        size_t messageSize;
        memcpy(&messageSize, header.data() + sizeof(int), sizeof(size_t));

        std::vector<char> temp(messageSize + 1, '\0');
        _buffer.popInto(temp.data(), messageSize);
        value = std::string(temp.data());
    }
    catch (const std::runtime_error& e)
    {
        throw;
    }
    return *this;
}

bool Message::isComplet()
{
    if (_buffer.size() < sizeof(int) + sizeof(size_t))
        return false;
    try
    {
        auto   header = _buffer.peek(sizeof(int) + sizeof(size_t));
        size_t messageSize;
        memcpy(&messageSize, header.data() + sizeof(int), sizeof(size_t));

        return (_buffer.size() >= sizeof(int) + sizeof(size_t) + messageSize);
    }
    catch (const std::runtime_error& e)
    {
        std::cout << "Exception in isComplet: " << e.what() << std::endl;
        return false;
    }
}

void Message::setType()
{
    try
    {
        auto header = _buffer.peek(sizeof(int));
        memcpy(&_type, header.data(), sizeof(int));
        return;
    }
    catch (const std::runtime_error& e)
    {
        throw;
    }
}

std::vector<unsigned char> Message::getData() const
{
    std::vector<unsigned char> data;
    size_t                     messageSize;

    auto header = _buffer.peek(sizeof(int) + sizeof(size_t));

    memcpy(&messageSize, header.data() + sizeof(int), sizeof(size_t));
    return _buffer.peek(sizeof(int) + sizeof(size_t) + messageSize);
}

std::vector<unsigned char> Message::popData()
{
    std::vector<unsigned char> data;
    size_t                     messageSize;

    auto header = _buffer.peek(sizeof(int) + sizeof(size_t));

    memcpy(&messageSize, header.data() + sizeof(int), sizeof(size_t));
    return _buffer.pop(sizeof(int) + sizeof(size_t) + messageSize);
}

void Message::reset()
{
    _buffer.clear();
}

Message::Type Message::type() const
{
    return _type;
}

RingBuffer& Message::data()
{
    return _buffer;
}