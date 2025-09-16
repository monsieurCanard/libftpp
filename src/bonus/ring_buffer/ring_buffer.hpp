#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

#include <stddef.h>

#include <iostream>
#include <stdexcept>
#include <vector>

#define MAX_BUFFER_SIZE 65550 // 16 KB

/**
 * @brief Classe de buffer circulaire (ring buffer) pour la gestion efficace des flux de données
 * @note Utilise un tableau circulaire pour stocker les données
 * @note Limité par la taille maximale définie (MAX_BUFFER_SIZE)
 *
 * @throw Lance des out_of_range en cas d'erreur
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