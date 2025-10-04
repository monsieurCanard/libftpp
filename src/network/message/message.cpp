#include "message.hpp"

Message::Message(Message::Type type) : _type(type) {}

Message& Message::operator<<(const std::string& value)
{
    try
    {
        size_t size = value.length();
        _buffer.pushInto(&size, sizeof(size_t));
        _buffer.pushInto(value.c_str(), size);
    }
    catch (const std::out_of_range& e)
    {
        throw;
    }
    return *this;
}

Message& Message::operator>>(std::string& value)
{
    try
    {
        size_t size;
        _buffer.popInto(&size, sizeof(size_t));

        std::vector<char> temp(size + 1, '\0');
        _buffer.popInto(temp.data(), size);

        value = std::string(temp.data());
    }
    catch (const std::out_of_range& e)
    {
        throw;
    }
    return *this;
}

std::string Message::messageToString() const
{
    try
    {
        auto data = getSerializedData();

        if (data.size() < sizeof(Message::Type) + sizeof(size_t))
            return "";

        size_t offset = sizeof(Message::Type);

        size_t dataSize;
        memcpy(&dataSize, data.data() + offset, sizeof(size_t));
        offset += sizeof(size_t);
        std::cout << "Data size: " << dataSize << std::endl;

        if (data.size() < offset + dataSize)
            return "";

        std::string result(reinterpret_cast<const char*>(data.data() + offset), dataSize);
        return result;
    }
    catch (std::out_of_range& e)
    {
        std::cerr << e.what() << std::endl;
        return "";
    }
}

bool Message::isComplet() const
{
    if (_buffer.size() < sizeof(Message::Type) + sizeof(size_t))
        return false;
    try
    {
        auto   header = _buffer.peek(sizeof(Message::Type) + sizeof(size_t));
        size_t messageSize;
        memcpy(&messageSize, header.data() + sizeof(Message::Type), sizeof(size_t));

        return (_buffer.size() >= sizeof(Message::Type) + sizeof(size_t) + messageSize);
    }
    catch (const std::out_of_range& e)
    {
        throw;
    }
}

std::vector<unsigned char> Message::getSerializedData() const
{
    std::vector<unsigned char> result;

    try
    {
        result.resize(sizeof(Message::Type));
        memcpy(result.data(), &_type, sizeof(Message::Type));

        auto   header = _buffer.peek(sizeof(size_t));
        size_t messageSize;
        memcpy(&messageSize, header.data(), sizeof(size_t));
        auto bufferData = _buffer.peek(sizeof(size_t) + messageSize);
        result.insert(result.end(), bufferData.begin(), bufferData.end());

        return result;
    }
    catch (std::out_of_range& e)
    {
        throw;
    }
}

std::vector<unsigned char> Message::popData()
{
    std::vector<unsigned char> data;
    size_t                     messageSize;

    try
    {
        auto header = _buffer.peek(sizeof(size_t));

        memcpy(&messageSize, header.data(), sizeof(size_t));
        return _buffer.pop(sizeof(size_t) + messageSize);
    }
    catch (std::out_of_range& e)
    {
        throw;
    }
}

int Message::popType()
{
    std::vector<unsigned char> data;
    int                        type;

    try
    {
        auto header = _buffer.pop(sizeof(int));
        memcpy(&type, header.data(), sizeof(int));
        return type;
    }
    catch (std::out_of_range& e)
    {
        throw;
    }
}

void Message::reset()
{
    _buffer.clear();
}

Message::Type Message::type() const
{
    return _type;
}

void Message::setType(Message::Type type)
{
    _type = type;
    return;
}

RingBuffer& Message::data()
{
    return _buffer;
}