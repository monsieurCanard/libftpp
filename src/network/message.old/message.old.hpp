#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <stddef.h>
#include <string.h>

#include <stdexcept>
#include <string>
#include <vector>

#include "../../bonus/ring_buffer/ring_buffer.hpp"

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
    RingBuffer _buffer;

public:
    Message(Type type);
    Message() : _fd(-1), _type(0) {}

    void setType(Message::Type type);

    template <typename T>
    Message& operator>>(T& value)
    {
        std::cout << " Const pop value of size " << sizeof(T) << std::endl;
        size_t size;
        _buffer.popInto(&size, sizeof(size_t));
        _buffer.popInto(&value, size);
        // std::cout << " Popped value " << value << " of size " << size << std::endl;
        return *this;
    }

    template <typename T>
    Message& operator<<(const T& value)
    {
        size_t size = sizeof(T); // Initialiser size d'abord
        _buffer.pushInto(&size, sizeof(size_t));
        _buffer.pushInto(&value, size);
        return *this;
    }

    template <typename T>
    const Message& operator>>(T& value) const
    {
        size_t size     = 0;
        auto   sizeData = _buffer.peek(sizeof(size_t));
        memcpy(&size, sizeData.data(), sizeof(size_t));

        std::cout << " Const peek value of size " << size << std::endl;
        auto valueData = _buffer.peek(sizeof(size_t) + size);
        memcpy(&value, valueData.data() + sizeof(size_t), size);

        return *this;
    }

    Message&       operator<<(const std::string& value);
    Message&       operator>>(std::string& value);
    const Message& operator>>(std::string& value) const;

    std::vector<unsigned char> popData();
    int                        popType();

    std::vector<unsigned char> getSerializedData() const;
    std::string                messageToString() const;

    template <typename T>
    void insertValue(T& value) const
    {
        auto data = getSerializedData();

        if (data.size() < sizeof(Message::Type) + sizeof(size_t))
            return;

        size_t offset = sizeof(Message::Type);

        size_t dataSize;
        memcpy(&dataSize, data.data() + offset, sizeof(size_t));
        offset += sizeof(size_t);

        if (data.size() < offset + dataSize)
            return;

        memcpy(&value, data.data() + sizeof(Message::Type) + sizeof(size_t), sizeof(T));
    }

    Message::Type type() const;
    RingBuffer&   data();
    bool          isComplet() const;
    void          reset();
    void          setMessageFd(int fd)
    {
        _fd = fd;
    }

    const int& getMessageFd()
    {
        return _fd;
    }
};
#endif