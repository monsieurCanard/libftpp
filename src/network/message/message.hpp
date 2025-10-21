#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <stddef.h>
#include <string.h>

#include <stdexcept>
#include <string>
#include <vector>

#include "../../data_structures/data_buffer/data_buffer.hpp"

/**
 * @brief Classe représentant un message structuré pour la communication réseau
 * @note Utilise un RingBuffer pour stocker les données du message
 * @note Le format de transfert du message est le suivant : [type (int)][taille (size_t)][données
 * (variable)]
 * @note A l'utilisation le buffer contient uniquement les données (sans le type) et le
 * Message::Type est defini dans _type
 *
 * @exception Lance des runtime_error en cas d'erreur
 *
 */
class Message
{
public:
    using Type = int;

private:
    int        _fd;
    Type       _type;
    DataBuffer _buffer;

public:
    Message(Type type);
    Message(int fd, Type type) : _fd(fd), _type(type) {}
    Message() : _fd(-1), _type(0) {}

    template <typename T>
    const Message& operator>>(T& value) const
    {
        _buffer >> value;
        return *this;
    }

    template <typename T>
    Message& operator<<(const T& value)
    {
        _buffer << value;
        return *this;
    }

    void                       appendBytes(const unsigned char* data, size_t len);
    std::vector<unsigned char> getSerializedData() const;

    void          setType(Message::Type type);
    Message::Type type() const;

    DataBuffer* getBuffer();

    void setMessageFd(int fd);

    void incr_cursor(size_t len) const;
    void decr_cursor(size_t len) const;

    bool isComplet();
    void reset();

    const int& getFd() const;
};
#endif