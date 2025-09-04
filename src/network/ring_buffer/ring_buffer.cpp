#include "ring_buffer.hpp"

RingBuffer::RingBuffer() : _buffer(MAX_BUFFER_SIZE) {}
RingBuffer::RingBuffer(size_t&& size_buffer) : _buffer(size_buffer) {}

bool RingBuffer::isEmpty()
{
    return _buffer.size() == 0;
}

bool RingBuffer::isfull()
{
    return _buffer.size() == _size;
}

size_t RingBuffer::size()
{
    return _buffer.size();
}

size_t RingBuffer::capacity()
{
    return _size;
}

void RingBuffer::push(const unsigned char& byte)
{
    if (isfull())
        throw std::runtime_error("trying to write on full buffer");
    _buffer[_head] = byte;
    _head          = (_head + 1) % _buffer.size();
    _size++;
}

void RingBuffer::push(const std::vector<unsigned char>& bytes)
{
    if (isfull())
        throw std::runtime_error("trying to write on full buffer");
    if (bytes.size() > _buffer.size() - _size)
        throw std::runtime_error("trying to read more than available in buffer");
    for (size_t i = 0; i < bytes.size(); i++)
    {
        _buffer[_head] = bytes[i];
        _head          = (_head + 1) % _buffer.size();
        _size++;
    }
}

unsigned char RingBuffer::pop()
{
    if (isEmpty())
        throw std::runtime_error("trying to read on empty buffer");
    unsigned char byte;
    byte  = _buffer[_tail];
    _tail = (_tail + 1) % _buffer.size();
    _size--;
    return byte;
}

std::vector<unsigned char> RingBuffer::pop(size_t&& size)
{
    if (isEmpty())
        throw std::runtime_error("trying to read on empty buffer");
    if (size > _size)
        throw std::runtime_error("trying to read more than available in buffer");
    std::vector<unsigned char> byte(size);
    for (size_t i = 0; i < size; i++)
    {
        byte.push_back(_buffer[_tail]);
        _tail = (_tail + 1) % _buffer.size();
        _size--;
    }

    return byte;
}

unsigned char RingBuffer::peek()
{
    if (isEmpty())
        throw std::runtime_error("trying to read on empty buffer");

    unsigned char byte;
    byte = _buffer[_tail];
    return byte;
}

std::vector<unsigned char> RingBuffer::peek(size_t&& size)
{
    if (isEmpty())
        throw std::runtime_error("trying to read on empty buffer");
    if (size > _size)
        throw std::runtime_error("trying to read more than available in buffer");
    std::vector<unsigned char> byte(size);
    size_t                     tmp_idx = _tail;
    for (size_t i = 0; i < size; i++)
    {
        byte.push_back(_buffer[tmp_idx]);
        tmp_idx = (tmp_idx + 1) % _buffer.size();
    }

    return byte;
}

void RingBuffer::clear()
{
    _buffer.clear();
    _tail = 0;
    _head = 0;
    _size = 0;
}