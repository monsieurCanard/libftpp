#ifndef DATA_BUFFER_HPP
#define DATA_BUFFER_HPP

#include <stdio.h>

#include <cstring>
#include <iostream>
#include <vector>

class DataBuffer
{
private:
    std::vector<unsigned char> buffer;

public:
    DataBuffer();
    ~DataBuffer();

    template <typename T>
    DataBuffer& operator<<(const T& value)
    {
        const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&value);
        buffer.insert(buffer.end(), ptr, ptr + sizeof(T));
        return *this;
    }

    template <typename T>
    DataBuffer& operator>>(T& value)
    {
        memcpy(&value, buffer.data(), sizeof(T));
        return *this;
    }
};

#endif
