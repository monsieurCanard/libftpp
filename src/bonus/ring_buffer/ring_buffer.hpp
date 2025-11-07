#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

#include <stddef.h>

#include <iostream>
#include <stdexcept>
#include <vector>

#define MAX_BUFFER_SIZE 65550 // 16 KB

/**
 * @brief RingBuffer class for managing a circular buffer of bytes.
 * It provides methods to push and pop bytes, check buffer status,
 * and retrieve buffer capacity and size.
 * @note The buffer operates in a circular manner, allowing efficient use of space.
 * @note The maximum buffer size is defined by MAX_BUFFER_SIZE constant.
 * @throw std::out_of_range when trying to push to a full buffer or pop from an empty buffer.
 *
 */
class RingBuffer
{
private:
    std::vector<unsigned char> _buffer;
    size_t                     _head;
    size_t                     _tail;
    size_t                     _size;

public:
    RingBuffer();
    RingBuffer(const size_t& size_buffer);

    void push(const unsigned char& byte);
    void push(const std::string& line);
    void push(const std::vector<unsigned char>& bytes);
    void pushInto(const void* data, const size_t& size);

    unsigned char              pop();
    std::vector<unsigned char> pop(const size_t& size);
    void                       popInto(void* data, const size_t& size);

    unsigned char              peek() const;
    std::vector<unsigned char> peek(const size_t& size) const;

    void clear();

    bool   isEmpty() const;
    bool   isFull() const;
    size_t size() const;
    size_t capacity() const;
    size_t capacityAvailable() const;
};

#endif