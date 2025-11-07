#include "ring_buffer.hpp"

RingBuffer::RingBuffer() : _buffer(MAX_BUFFER_SIZE), _head(0), _tail(0), _size(0) {}
RingBuffer::RingBuffer(const size_t& size_buffer)
    : _buffer(size_buffer), _head(0), _tail(0), _size(0)
{
}

void RingBuffer::push(const std::string& line)
{
    if (isFull())
        throw std::out_of_range("trying to write on full buffer");

    std::vector<unsigned char> tmp(line.begin(), line.end());
    push(tmp);
}

void RingBuffer::push(const unsigned char& byte)
{
    if (isFull())
        throw std::out_of_range("trying to write on full buffer");

    _buffer[_head] = byte;
    _head          = (_head + 1) % _buffer.size();
    _size++;
}

void RingBuffer::push(const std::vector<unsigned char>& bytes)
{
    if (isFull())
        throw std::out_of_range("trying to write on full buffer, try to increase size");

    if (bytes.size() > _buffer.size() - _size)
        throw std::out_of_range("trying to read more than available in buffer");

    for (size_t i = 0; i < bytes.size(); ++i)
    {
        _buffer[_head] = bytes[i];
        _head          = (_head + 1) % _buffer.size();
        _size++;
    }
}

unsigned char RingBuffer::pop()
{
    if (isEmpty())
        throw std::out_of_range("trying to read on empty buffer");

    unsigned char byte;
    byte  = _buffer[_tail];
    _tail = (_tail + 1) % _buffer.size();
    _size--;

    return byte;
}

std::vector<unsigned char> RingBuffer::pop(const size_t& size)
{
    if (size > _size)
        throw std::out_of_range("trying to read more than available in buffer or empty buffer");

    std::vector<unsigned char> byte(size);
    for (size_t i = 0; i < size; ++i)
    {
        byte[i] = _buffer[_tail];
        _tail   = (_tail + 1) % _buffer.size();
        _size--;
    }

    return byte;
}

void RingBuffer::pushInto(const void* data, const size_t& size)
{
    if (capacityAvailable() < size)
        throw std::out_of_range("trying to put more than available in buffer");

    const unsigned char* ptr = reinterpret_cast<const unsigned char*>(data);
    for (size_t i = 0; i < size; ++i)
    {
        _buffer[_head] = ptr[i];
        _head          = (_head + 1) % _buffer.size();
        _size++;
    }
}

void RingBuffer::popInto(void* data, const size_t& size)
{
    if (size > _size)
        throw std::out_of_range("trying to read more than available in buffer or empty buffer");

    unsigned char* ptr = reinterpret_cast<unsigned char*>(data);
    for (size_t i = 0; i < size; ++i)
    {
        ptr[i] = _buffer[_tail];
        _tail  = (_tail + 1) % _buffer.size();
        _size--;
    }
}

unsigned char RingBuffer::peek() const
{
    if (isEmpty())
        throw std::out_of_range("trying to read on empty buffer");

    unsigned char byte;
    byte = _buffer[_tail];
    return byte;
}

std::vector<unsigned char> RingBuffer::peek(const size_t& size) const
{
    if (isEmpty())
        throw std::out_of_range("trying to read on empty buffer");

    if (size > _size)
        throw std::out_of_range("trying to read more than available in buffer");

    std::vector<unsigned char> byte(size);
    size_t                     tmp_idx = _tail;

    for (size_t i = 0; i < size; ++i)
    {
        byte[i] = _buffer[tmp_idx];
        tmp_idx = (tmp_idx + 1) % _buffer.size();
    }

    return byte;
}

bool RingBuffer::isEmpty() const
{
    return _size == 0;
}

bool RingBuffer::isFull() const
{
    return _buffer.size() == _size;
}

size_t RingBuffer::size() const
{
    return _size;
}

size_t RingBuffer::capacity() const
{
    return _buffer.size();
}

size_t RingBuffer::capacityAvailable() const
{
    return _buffer.size() - _size;
}

void RingBuffer::clear()
{
    _tail = 0;
    _head = 0;
    _size = 0;
}