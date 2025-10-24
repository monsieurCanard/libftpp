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

    const std::vector<unsigned char> data() const;
    void                             increaseCursor(size_t amount) const;
    void                             decreaseCursor(size_t amount) const;

    void   reset() const;
    void   clear();
    size_t size() const;

    void append(const unsigned char* data, size_t len);

    DataBuffer& operator<<(std::string& value);
    DataBuffer& operator>>(std::string& value);

    // Serialization
    template <typename T>
    DataBuffer& operator<<(const T& value)
    {
        const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&value);
        _buffer.insert(_buffer.end(), ptr, ptr + sizeof(T));

        std::cout << "Wrote value: " << value << " at cursor: " << _buffer.size() - sizeof(T)
                  << std::endl;
        return *this;
    }

    // Deserialization -- LECTURE
    template <typename T>
    const DataBuffer& operator>>(T& value) const
    {
        if (sizeof(T) + _cursor > _buffer.size())
            throw std::out_of_range("Buffer overflow on read");
        std::memcpy(&value, _buffer.data() + _cursor, sizeof(T));

        std::cout << "Read value: " << value << " at cursor: " << _cursor << std::endl;
        _cursor += sizeof(T);

        std::cout << *reinterpret_cast<const T*>(_buffer.data() + _cursor) << std::endl;
        return *this;
    }
};

#endif
