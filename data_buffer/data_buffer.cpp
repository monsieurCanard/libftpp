#include "data_buffer.hpp"

DataBuffer::DataBuffer() {}

DataBuffer::~DataBuffer() {}

// template <typename T>
// DataBuffer& DataBuffer::operator<<(const T& value)
// {
//     const unsigned char* ptr = reinterpret_cast<unsigned char>(&value);
//     buffer.insert(buffer.end(), ptr, ptr + sizeof(T));
//     return *this;
// }

// template <typename T>
// DataBuffer& DataBuffer::operator>>(const T& value)
// {
//     return *this;
// }
