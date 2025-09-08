#include "message.hpp"

Message::Message(Type type) : _type(type) {}

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
    size_t size;
    *this >> size;
    if (size > _buffer.capacityAvailable())
        throw std::runtime_error("Read out of Buffer !");
    try
    {
        std::vector<char> temp(size + 1, '\0');
        _buffer.popInto(temp.data(), size); // Lire les données

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
    std::cout << "BUffer size = " << _buffer.size() << std::endl;
    if (_buffer.size() < sizeof(int) + sizeof(size_t))
        return false;
    try
    {
        auto   header = _buffer.peek(sizeof(int) + sizeof(size_t));
        size_t messageSize;
        memcpy(&messageSize, header.data() + sizeof(int), sizeof(size_t));
        std::cout << "Message size = " << sizeof(int) + sizeof(size_t) + messageSize << std::endl;

        return (_buffer.size() <= sizeof(int) + sizeof(size_t) + messageSize);
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
    auto                       header = _buffer.peek(sizeof(int) + sizeof(size_t));
    size_t                     messageSize;
    memcpy(&messageSize, header.data() + sizeof(int), sizeof(size_t));
    return _buffer.peek(sizeof(int) + sizeof(size_t) + messageSize);
}

std::vector<unsigned char> Message::popData()
{
    std::vector<unsigned char> data;
    auto                       header = _buffer.peek(sizeof(int) + sizeof(size_t));
    size_t                     messageSize;
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