#ifndef DATA_BUFFER_HPP
#define DATA_BUFFER_HPP

#include <stdio.h>

#include <cstring>
#include <iostream>
#include <vector>

/**
 * @brief A simple data buffer for serialization and deserialization for simple data types and
 * std::string.
 *
 * @throw std::out_of_range Thrown when trying to read more data than available in the buffer.
 */
class DataBuffer
{
private:
    std::vector<unsigned char> _buffer;
    size_t                     _cursor;

public:
    DataBuffer();
    ~DataBuffer() = default;

    void reset();
    void clear();

    DataBuffer& operator<<(const std::string& value);
    DataBuffer& operator>>(std::string& value);

    // Serialization
    template <typename T>
    DataBuffer& operator<<(const T& value)
    {
        const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&value);
        _buffer.insert(_buffer.end(), ptr, ptr + sizeof(T));
        return *this;
    }

    // Deserialization -- LECTURE
    template <typename T>
    DataBuffer& operator>>(T& value)
    {
        if (sizeof(T) + _cursor > _buffer.size())
            throw std::out_of_range("Buffer overflow on read");

        memcpy(&value, _buffer.data() + _cursor, sizeof(T));
        _cursor += sizeof(T);
        return *this;
    }
};

#endif
