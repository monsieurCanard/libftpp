#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

#include <stddef.h>

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
    RingBuffer(size_t&& size_buffer);

    void push(unsigned char byte);

    unsigned char pop();

    std::vector<unsigned char> pop(size_t&& size);
    unsigned char              peek();

    std::vector<unsigned char> peek(size_t&& size);
    void                       clear();

    bool   isEmpty();
    bool   isfull();
    size_t size();
    size_t capacity();
};

#endif