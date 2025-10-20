#ifndef DATA_BUFFER_HPP
#define DATA_BUFFER_HPP

#include <stdio.h>

#include <cstring>
#include <iostream>
#include <vector>

/**
 * @brief A simple LIFO data buffer for serialization and deserialization for simple data types and
 * std::string.
 *
 * @throw std::out_of_range Thrown when trying to read more data than available in the buffer.
 */
class DataBuffer
{
private:
    std::vector<unsigned char> _buffer;
    mutable size_t             _cursor;

public:
    DataBuffer();
    ~DataBuffer() = default;

    const std::vector<unsigned char> data() const
    {
        return std::vector<unsigned char>(_buffer.begin() + _cursor, _buffer.end());
    }

    void   reset() const;
    void   clear();
    size_t size() const
    {
        return _buffer.size();
    }

    void append(const unsigned char* data, size_t len);

    DataBuffer& operator<<(const std::string& value);
    DataBuffer& operator>>(std::string& value);

    // Serialization
    template <typename T>
    DataBuffer& operator<<(const T& value)
    {
        const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&value);
        std::cout << " Push value of size " << sizeof(T) << std::endl;
        _buffer.insert(_buffer.end(), ptr, ptr + sizeof(T));
        return *this;
    }

    // Deserialization -- LECTURE
    template <typename T>
    const DataBuffer& operator>>(T& value) const
    {
        if (sizeof(T) + _cursor > _buffer.size())
            throw std::out_of_range("Buffer overflow on read");

        value = *reinterpret_cast<const T*>(_buffer.data() + _cursor);
        _cursor += sizeof(T);
        return *this;
    }
};

#endif
