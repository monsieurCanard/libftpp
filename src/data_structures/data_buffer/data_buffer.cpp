#include "data_buffer.hpp"

DataBuffer::DataBuffer() : _cursor(0) {}

void DataBuffer::reset() const
{
    _cursor = 0;
}

void DataBuffer::clear()
{
    _buffer.clear();
    _cursor = 0;
}

void DataBuffer::append(const unsigned char* data, size_t len)
{
    if (len == 0 || data == nullptr)
        return;
    _buffer.insert(_buffer.end(), data, data + len);
}

DataBuffer& DataBuffer::operator<<(std::string& value)
{
    size_t size = value.length();
    *this << size;

    _buffer.insert(_buffer.end(), value.begin(), value.end());
    std::cout << "Wrote string: " << value << " at cursor: " << *_buffer.end() << std::endl;
    return *this;
}

DataBuffer& DataBuffer::operator>>(std::string& value)
{
    size_t size;

    *this >> size;
    if (size + _cursor > _buffer.size())
        throw std::out_of_range("Buffer overflow on read");

    value.assign(_buffer.begin() + _cursor, _buffer.begin() + _cursor + size);

    _cursor += size;

    return *this;
}

size_t DataBuffer::size() const
{
    return _buffer.size() - _cursor;
}

const std::vector<unsigned char> DataBuffer::data() const
{
    return std::vector<unsigned char>(_buffer.begin() + _cursor, _buffer.end());
}

void DataBuffer::increaseCursor(size_t amount) const
{
    if (_cursor + amount > _buffer.size())
        throw std::out_of_range("Buffer overflow on increaseCursor");

    _cursor += amount;
}

void DataBuffer::decreaseCursor(size_t amount) const
{
    if (amount > _cursor)
        throw std::out_of_range("Buffer underflow on decreaseCursor");

    _cursor -= amount;
}