#include "message.hpp"

Message::Message(Message::Type type) : _type(type) {}

void Message::appendBytes(const unsigned char* data, size_t len)
{
    _buffer.append(data, len);
}

bool Message::isComplet()
{
    if (_buffer.size() < sizeof(Message::Type) + sizeof(size_t))
        return false;

    Message::Type type;
    _buffer >> type;

    size_t messageSize;
    _buffer >> messageSize;

    _buffer.decreaseCursor(sizeof(Message::Type) + sizeof(size_t));

    return (_buffer.size() >= sizeof(Message::Type) + sizeof(size_t) + messageSize);
}

std::vector<unsigned char> Message::getSerializedData() const
{
    std::vector<unsigned char> result;
    size_t                     totalSize = sizeof(Message::Type) + sizeof(size_t) + _buffer.size();
    result.reserve(totalSize);

    result.insert(result.end(),
                  reinterpret_cast<const unsigned char*>(&_type),
                  reinterpret_cast<const unsigned char*>(&_type) + sizeof(Message::Type));

    size_t bufferSize = _buffer.size();
    result.insert(result.end(),
                  reinterpret_cast<const unsigned char*>(&bufferSize),
                  reinterpret_cast<const unsigned char*>(&bufferSize) + sizeof(size_t));

    // Mettre les données
    auto bufData = _buffer.data(); // récupérer une seule fois le vector tempora
    result.insert(result.end(), bufData.begin(), bufData.end());

    return result;
}

void Message::reset()
{
    _buffer.reset();
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

void Message::setMessageFd(int fd)
{
    _fd = fd;
}

void Message::incr_cursor(size_t len) const
{
    _buffer.increaseCursor(len);
}

void Message::decr_cursor(size_t len) const
{
    _buffer.decreaseCursor(len);
}

const int& Message::getFd() const
{
    return _fd;
}

DataBuffer* Message::getBuffer()
{
    return &_buffer;
}