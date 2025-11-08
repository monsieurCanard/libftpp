#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <stddef.h>
#include <string.h>

#include <stdexcept>
#include <string>
#include <vector>

#include "../../data_structures/data_buffer/data_buffer.hpp"

/**
 * @brief Class representing a structured message for network communication.
 *
 * This class provides a convenient way to handle structured messages in network protocols.
 * It uses a DataBuffer internally to store message data and supports serialization for
 * network transmission.
 *
 * @note Uses a DataBuffer to store message data efficiently
 * @note The transfer format is: [type (int)][size (size_t)][data (variable)]
 * @note During usage, the buffer contains only the data (without type), and the
 *       Message::Type is stored separately in _type
 * @note Supports stream operators (<<, >>) for easy data insertion and extraction
 *
 * @code
 * // Create a message with a specific type
 * Message msg(1001);
 *
 * // Add data using stream operators
 * msg << std::string("Hello") << 42 << 3.14f;
 *
 * // Serialize for network transmission
 * auto serialized = msg.getSerializedData();
 *
 * // Extract data (order matters!)
 * std::string text;
 * int number;
 * float value;
 * msg >> text >> number >> value;
 * @endcode
 *
 * @throws std::runtime_error on serialization/deserialization errors
 * @see DataBuffer for underlying data storage mechanism
 */
class Message
{
public:
    using Type = int;

private:
    int        _fd;
    Type       _type;
    DataBuffer _buffer;

public:
    Message(Type type);
    Message(int fd, Type type) : _fd(fd), _type(type) {}
    Message() : _fd(-1), _type(0) {}

    template <typename T>
    const Message& operator>>(T& value) const
    {
        _buffer >> value;
        return *this;
    }

    template <typename T>
    Message& operator<<(const T& value)
    {
        _buffer << value;
        return *this;
    }

    void                       appendBytes(const unsigned char* data, size_t len);
    std::vector<unsigned char> getSerializedData() const;

    void          setType(Message::Type type);
    Message::Type type() const;

    DataBuffer* getBuffer();

    void setMessageFd(int fd);

    void incr_cursor(size_t len) const;
    void decr_cursor(size_t len) const;

    bool isComplet();
    void reset();

    const int& getFd() const;
};
#endif