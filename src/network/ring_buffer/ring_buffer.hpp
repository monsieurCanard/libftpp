#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

#include <stddef.h>

#include <iostream>
#include <stdexcept>
#include <vector>

#define MAX_BUFFER_SIZE 16000 // 16 KB

class RingBuffer
{
private:
    std::vector<unsigned char> _buffer;
    size_t                     _tail = 0;
    size_t                     _head = 0;
    size_t                     _size = 0; // Bytes actuellement ecrient sur le buffer

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

    unsigned char peek() const;

    std::vector<unsigned char> peek(const size_t& size) const;
    void                       clear();

    bool   isEmpty() const;
    bool   isFull() const;
    size_t size();
    size_t capacity();
    size_t capacityAvailable();
};

#endif