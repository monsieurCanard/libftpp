#include "data_buffer.hpp"

DataBuffer::DataBuffer() : _cursor(0) {}

DataBuffer::~DataBuffer() {}

void DataBuffer::reset()
{
    _cursor = 0;
}

DataBuffer& DataBuffer::operator<<(const std::string& value)
{
    size_t size = value.length();
    *this << size;
    _buffer.insert(_buffer.end(), value.begin(), value.end());
    return *this;
}

DataBuffer& DataBuffer::operator>>(std::string& value)
{

    size_t size;
    *this >> size;
    if (size + _cursor > _buffer.size())
        throw std::runtime_error("Read out of Buffer !");
    value.assign(_buffer.begin() + _cursor, _buffer.begin() + _cursor + size);
    _cursor += size;
    return *this;
}