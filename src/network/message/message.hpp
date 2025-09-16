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
        try
        {
            _buffer.pop(sizeof(size_t));
            _buffer.popInto(&value, sizeof(T));
        }
        catch (const std::out_of_range& e)
        {
            throw;
        }
        return *this;
    }

    template <typename T>
    Message& operator<<(const T& value)
    {
        try
        {
            size_t size = sizeof(T);
            _buffer.pushInto(&size, sizeof(size_t));
            _buffer.pushInto(&value, sizeof(T));
        }
        catch (const std::out_of_range& e)
        {
            throw;
        }
        return *this;
    }

    Message& operator<<(const std::string& value);
    Message& operator>>(std::string& value);

    std::vector<unsigned char> popData();
    int                        popType();

    std::vector<unsigned char> getSerializedData() const;
    std::string                messageToString() const;

    template <typename T>
    void insertValue(T& value) const
    {
        try
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
        catch (std::out_of_range& e)
        {
            throw;
        }
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