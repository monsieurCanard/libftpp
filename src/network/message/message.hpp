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
    int        _fd;

public:
    Message(Type type);
    Message() {}
    void setType(int type);

    template <typename T>
    Message& operator>>(T& value)
    {
        if (_buffer.isEmpty())
            throw std::runtime_error("No data in buffer");

        try
        {
            // TODO : Voir si ca vaux la peine de verifier si la data est coherente avec le sizeT
            //  auto   header = _buffer.peek(sizeof(size_t));
            //  size_t dataSize;
            //  memcpy(&dataSize, header.data(), sizeof(size_t));

            // if (dataSize != sizeof(T))
            //     throw std::runtime_error("Size mismatch for type");

            _buffer.pop(sizeof(size_t));

            _buffer.popInto(&value, sizeof(T));
            // std::cout << _buffer.pop(sizeof(size_t)) << std::endl;
        }
        catch (const std::runtime_error& e)
        {
            throw;
        }
        return *this;
    }

    template <typename T>
    Message& operator<<(const T& value)
    {
        try
        {
            size_t size = sizeof(T);
            _buffer.pushInto(&size, sizeof(size_t));
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
    int                        popType();

    std::vector<unsigned char> getSerializedData() const;

    Message::Type type() const;
    RingBuffer&   data();
    bool          isComplet();
    void          reset();
    void          setMessageFd(int fd)
    {
        _fd = fd;
    }

    const int& getMessageFd()
    {
        return _fd;
    }
};
#endif